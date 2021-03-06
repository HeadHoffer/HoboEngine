/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
//#include <initializer_list>
//#include <jni.h>
//#include <errno.h>
//#include <cassert>

//#include <EGL/egl.h>
//#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <Window.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>
#include <AndroidWindow.h>



#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

//Our saved state data.
struct saved_state
{
    float angle;
    int32_t x;
    int32_t y;
};

//Shared state for our app.
struct AndroidEngine
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    //EGLDisplay display;
    //EGLSurface surface;
    //EGLContext context;

    engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem;
    engine::Ref<engine::Application> application;
    engine::Ref<engine::Window> window;
    engine::Ref<engine::ElapsedTimer> frameTimer;


    //File reading.
    //AAssetManager* assetManager = app->activity->assetManager;
    //AAsset* file = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);

    struct saved_state state;
};

//Initialize an EGL context for the current display.
static int engine_init_display(struct AndroidEngine* androidEngine)
{
    androidEngine->window = new engine::AndroidWindow(androidEngine->app->window);
    androidEngine->graphicsSystem = new engine::OpenGLES2GraphicsSystem(androidEngine->window);
    androidEngine->application = new engine::Application();

    androidEngine->frameTimer = new engine::ElapsedTimer();
    androidEngine->frameTimer->reset();

    return 0;
}

//Just the current frame in the display.
static void engine_draw_frame(struct AndroidEngine* androidEngine)
{
    if(androidEngine == 0 || androidEngine->application == 0)
    {
        return;
    }

    float deltaTime = androidEngine->frameTimer->getTime();
    androidEngine->frameTimer->reset();

    androidEngine->application->update(deltaTime);
    androidEngine->application->render(androidEngine->window, androidEngine->graphicsSystem);

//    if (androidEngine->display == NULL)
//    {
//        //No display.
//        return;
//    }

//    //Just fill the screen with a color.
//    glClearColor(((float)androidEngine->state.x)/androidEngine->width, androidEngine->state.angle,
//                 ((float)androidEngine->state.y)/androidEngine->height, 1);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    eglSwapBuffers(androidEngine->display, androidEngine->surface);
}

//Tear down the EGL context currently associated with the display.
static void engine_term_display(struct AndroidEngine* androidEngine)
{
    androidEngine->graphicsSystem = 0;
    androidEngine->application = 0;
    androidEngine->window = 0;
    androidEngine->frameTimer = 0;

//    if (androidEngine->display != EGL_NO_DISPLAY)
//    {
//        eglMakeCurrent(androidEngine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
//        if (androidEngine->context != EGL_NO_CONTEXT)
//        {
//            eglDestroyContext(androidEngine->display, androidEngine->context);
//        }
//        if (androidEngine->surface != EGL_NO_SURFACE)
//        {
//            eglDestroySurface(androidEngine->display, androidEngine->surface);
//        }
//        eglTerminate(androidEngine->display);
//    }
//
//    androidEngine->animating = 0;
//    androidEngine->display = EGL_NO_DISPLAY;
//    androidEngine->context = EGL_NO_CONTEXT;
//    androidEngine->surface = EGL_NO_SURFACE;
}

//Process the next input event.
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    struct AndroidEngine* androidEngine = (struct AndroidEngine*)app->userData;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        androidEngine->animating = 1;
        androidEngine->state.x = AMotionEvent_getX(event, 0);
        androidEngine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

//Process the next main command.
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct AndroidEngine* androidEngine = (struct AndroidEngine*)app->userData;
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            //The system has asked us to save our current state.  Do so.
            androidEngine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)androidEngine->app->savedState) = androidEngine->state;
            androidEngine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            //The window is being shown, get it ready.
            if (androidEngine->app->window != NULL)
            {
                engine_init_display(androidEngine);
                engine_draw_frame(androidEngine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            //The window is being hidden or closed, clean it up.
            engine_term_display(androidEngine);
            break;
        case APP_CMD_GAINED_FOCUS:
            //When our app gains focus, we start monitoring the accelerometer.
            if (androidEngine->accelerometerSensor != NULL)
            {
                ASensorEventQueue_enableSensor(androidEngine->sensorEventQueue,
                                               androidEngine->accelerometerSensor);
                //We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(androidEngine->sensorEventQueue,
                                               androidEngine->accelerometerSensor,
                                               (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            //When our app loses focus, we stop monitoring the accelerometer.
            //This is to avoid consuming battery while not being used.
            if (androidEngine->accelerometerSensor != NULL)
            {
                ASensorEventQueue_disableSensor(androidEngine->sensorEventQueue,
                                                androidEngine->accelerometerSensor);
            }
            //Also stop animating.
            androidEngine->animating = 0;
            engine_draw_frame(androidEngine);
            break;
    }
}

//This is the main entry point of a native application that is using
//android_native_app_glue.  It runs in its own thread, with its own
//event loop for receiving input events and doing other things.
void android_main(struct android_app* state)
{
    struct AndroidEngine androidEngine;

    //Make sure glue isn't stripped.
    app_dummy();

    memset(&androidEngine, 0, sizeof(androidEngine));
    state->userData = &androidEngine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    androidEngine.app = state;

    // Prepare to monitor accelerometer
    androidEngine.sensorManager = ASensorManager_getInstance();
    androidEngine.accelerometerSensor = ASensorManager_getDefaultSensor(androidEngine.sensorManager,
                                                                        ASENSOR_TYPE_ACCELEROMETER);
    androidEngine.sensorEventQueue = ASensorManager_createEventQueue(androidEngine.sensorManager,
                                                                     state->looper, LOOPER_ID_USER,
                                                                     NULL, NULL);

    if (state->savedState != NULL)
    {
        //We are starting with a previous saved state; restore from it.
        androidEngine.state = *(struct saved_state*)state->savedState;
    }

    //Loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        //If not animating, we will block forever waiting for events.
        //If animating, we loop until all events are read, then continue
        //to draw the next frame of animation.
        while ((ident=ALooper_pollAll(androidEngine.animating ? 0 : -1, NULL, &events,
                                      (void**)&source)) >= 0)
        {
            //Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }

            //If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER)
            {
                if (androidEngine.accelerometerSensor != NULL)
                {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(androidEngine.sensorEventQueue,
                                                       &event, 1) > 0)
                    {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                             event.acceleration.x, event.acceleration.y,
                             event.acceleration.z);
                    }
                }
            }

            //Check if we are exiting.
            if (state->destroyRequested != 0)
            {
                engine_term_display(&androidEngine);
                return;
            }
        }

        if (androidEngine.animating)
        {
            //Done with events; draw next animation frame.
            androidEngine.state.angle += .01f;
            if (androidEngine.state.angle > 1)
            {
                androidEngine.state.angle = 0;
            }

            //Drawing is throttled to the screen update rate, so there
            //Is no need to do timing here.
            engine_draw_frame(&androidEngine);
        }
    }
}
//END_INCLUDE(all)
