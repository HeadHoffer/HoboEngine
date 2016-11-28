#include <PNGFile.h>

namespace engine
{
	PNGFile::PNGFile() : Object()
	{
	}

	PNGFile::~PNGFile()
	{
	}

	GLuint PNGFile::loadTexture(const std::string filename, int &width, int &height)
	{
		// header for testing if it is a png
		png_byte header[8];

		//open file as binary
		FILE *fp = fopen(filename.c_str(), "rb");
		if (!fp)
		{
			return TEXTURE_LOAD_ERROR;
		}

		//read the header
		fread(header, 1, 8, fp);

		//test if png
		int is_png = !png_sig_cmp(header, 0, 8);
		if (!is_png)
		{
			fclose(fp);
			return TEXTURE_LOAD_ERROR;
		}

		//create png struct
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
		if (!png_ptr)
		{
			fclose(fp);
			return (TEXTURE_LOAD_ERROR);
		}

		//create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			return (TEXTURE_LOAD_ERROR);
		}

		//create png info struct
		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
			fclose(fp);
			return (TEXTURE_LOAD_ERROR);
		}


		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return (TEXTURE_LOAD_ERROR);
		}

		//init png reading
		png_init_io(png_ptr, fp);

		//let libpng know you already read the first 8 bytes
		png_set_sig_bytes(png_ptr, 8);

		// read all the info up to the image data
		png_read_info(png_ptr, info_ptr);

		//variables to pass to get info
		int bit_depth, color_type;
		png_uint_32 twidth, theight;

		// get info about png
		png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
			NULL, NULL, NULL);

		//update width and height based on png info
		width = twidth;
		height = theight;

		// Update the png info struct.
		png_read_update_info(png_ptr, info_ptr);

		// Row size in bytes.
		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		// Allocate the image_data as a big block, to be given to opengl
		png_byte *image_data = new png_byte[rowbytes * height];
		if (!image_data)
		{
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			return TEXTURE_LOAD_ERROR;
		}

		//row_pointers is for pointing to image_data for reading the png with libpng
		png_bytep *row_pointers = new png_bytep[height];
		if (!row_pointers) {
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			delete[] image_data;
			fclose(fp);
			return TEXTURE_LOAD_ERROR;
		}

		// set the individual row_pointers to point at the correct offsets of image_data
		for (int i = 0; i < height; ++i)
			row_pointers[height - 1 - i] = image_data + i * rowbytes;

		//read the png into image_data through row_pointers
		png_read_image(png_ptr, row_pointers);

		//Now generate the OpenGL texture object
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		delete[] row_pointers;
		fclose(fp);

		return texture;
	}

	//GLuint PNGFile::returnTexture()
	//{
	//	return m_pngTexture;
	//}

	//void PNGFile::readPNGFile(char * filename)
	//{
	//	FILE *fp = fopen(filename, "rb");

	//	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	//	if (!png) abort();

	//	png_infop info = png_create_info_struct(png);
	//	if (!info) abort();

	//	if (setjmp(png_jmpbuf(png))) abort();

	//	png_init_io(png, fp);

	//	png_read_info(png, info);

	//	width = png_get_image_width(png, info);
	//	height = png_get_image_height(png, info);
	//	color_type = png_get_color_type(png, info);
	//	bit_depth = png_get_bit_depth(png, info);

	//	if (bit_depth == 16)
	//		png_set_strip_16(png);

	//	if (color_type == PNG_COLOR_TYPE_PALETTE)
	//		png_set_palette_to_rgb(png);

	//	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	//	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	//		png_set_expand_gray_1_2_4_to_8(png);

	//	if (png_get_valid(png, info, PNG_INFO_tRNS))
	//		png_set_tRNS_to_alpha(png);

	//	// These color_type don't have an alpha channel then fill it with 0xff.
	//	if (color_type == PNG_COLOR_TYPE_RGB ||
	//		color_type == PNG_COLOR_TYPE_GRAY ||
	//		color_type == PNG_COLOR_TYPE_PALETTE)
	//		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	//	if (color_type == PNG_COLOR_TYPE_GRAY ||
	//		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	//		png_set_gray_to_rgb(png);

	//	png_read_update_info(png, info);

	//	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	//	for (int y = 0; y < height; y++)
	//	{
	//		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	//	}

	//	png_read_image(png, row_pointers);

	//	png_destroy_read_struct(&png, &info, NULL);
	//	png = NULL;
	//	info = NULL;

	//	fclose(fp);
	//}

	//void PNGFile::writePNGFile(char *filename)
	//{
	//	int y;

	//	FILE *fp = fopen(filename, "wb");
	//	if (!fp)
	//	{
	//		printf("!fp");
	//		abort();
	//	}

	//	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	//	if (!png)
	//	{
	//		printf("!png");
	//		abort();
	//	}

	//	png_infop info = png_create_info_struct(png);
	//	if (!info)
	//	{
	//		printf("!info");
	//		abort();
	//	}

	//	if (setjmp(png_jmpbuf(png)))

	//	{
	//		abort();
	//	}

	//	png_init_io(png, fp);

	//	png_set_IHDR(
	//		png,
	//		info,
	//		width, height,
	//		8,
	//		PNG_COLOR_TYPE_RGBA,
	//		PNG_INTERLACE_NONE,
	//		PNG_COMPRESSION_TYPE_DEFAULT,
	//		PNG_FILTER_TYPE_DEFAULT
	//		);
	//	png_write_info(png, info);

	//	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	//	// Use png_set_filler().
	//	//png_set_filler(png, 0, PNG_FILLER_AFTER);

	//	png_write_image(png, row_pointers);
	//	png_write_end(png, NULL);

	//	for (int y = 0; y < height; y++) 
	//	{
	//		free(row_pointers[y]);
	//	}
	//	free(row_pointers);


	//	if (png && info)
	//		png_destroy_write_struct(&png, &info);

	//	fclose(fp);
	//}

	//void PNGFile::processPNGFile()
	//{
	//	for (int y = 0; y < height; y++)
	//	{
	//		png_bytep row = row_pointers[y];
	//		for (int x = 0; x < width; x++) 
	//		{
	//			png_bytep px = &(row[x * 4]);
	//			
	//			printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
	//		}
	//	}
	//}

	//int PNGFile::stuffHappnes(int argc, char *argv[])
	//{
	//	readPNGFile(argv[1]);
	//	processPNGFile();
	//	writePNGFile(argv[2]);
	//	return 0;
	//}
}