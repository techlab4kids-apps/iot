

#define LOGO_WIDTH 128
#define LOGO_HEIGHT 48
static unsigned char LOGO_bits[] = {

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x3f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0xf0, 0x83, 0x1f,
    0xf0, 0x7f, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x80, 0x03, 0x03, 0x00,
    0x00, 0xf0, 0x83, 0x1f, 0xf8, 0x73, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00,
    0x80, 0x83, 0x07, 0x00, 0x00, 0xf0, 0x83, 0x3f, 0xf8, 0x61, 0x00, 0x80,
    0x1f, 0x00, 0x00, 0x00, 0x80, 0xc3, 0x07, 0x00, 0x00, 0xf0, 0x83, 0x1f,
    0xfc, 0x21, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xc0, 0xe7, 0x0f, 0x00,
    0x00, 0xf8, 0x01, 0x1f, 0xfc, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00,
    0xc0, 0xe7, 0x0f, 0x00, 0x00, 0xf8, 0x01, 0x06, 0xfc, 0x00, 0x00, 0xc0,
    0x0f, 0x00, 0x00, 0x00, 0xe0, 0xf7, 0x0f, 0x00, 0x00, 0xf8, 0x01, 0x00,
    0xfc, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x1f, 0x01,
    0x00, 0xf8, 0x01, 0x00, 0xfc, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00,
    0xe0, 0xff, 0x1f, 0x01, 0x00, 0xfc, 0xc1, 0x0f, 0xfe, 0x07, 0xfe, 0xe0,
    0x07, 0x7e, 0xf0, 0x01, 0xf0, 0xff, 0xbf, 0x03, 0x00, 0xfc, 0xc0, 0x0f,
    0xfe, 0x02, 0xff, 0xe1, 0x07, 0x7e, 0xf8, 0x01, 0xf0, 0xff, 0xbf, 0x03,
    0x00, 0xfc, 0xc0, 0x0f, 0x7e, 0xc0, 0xcf, 0xe1, 0x07, 0x3f, 0xf8, 0x01,
    0xf0, 0xff, 0xff, 0x07, 0x00, 0xfc, 0xe0, 0x07, 0x7e, 0xe0, 0xc7, 0xe3,
    0x07, 0x3f, 0xf8, 0x01, 0xf0, 0xff, 0xff, 0x07, 0x00, 0xfc, 0xe0, 0x07,
    0x7e, 0xe0, 0xc3, 0xf3, 0x07, 0x3f, 0xf8, 0x01, 0xf8, 0xff, 0xff, 0x0f,
    0x00, 0x7e, 0xe0, 0x07, 0x3f, 0xf0, 0xc3, 0xf1, 0x03, 0x3f, 0xfc, 0x00,
    0xf8, 0xff, 0xff, 0x0f, 0x00, 0x7e, 0xe0, 0x07, 0x3f, 0xf0, 0xc1, 0xf1,
    0x03, 0x3f, 0xfc, 0x00, 0xf8, 0xff, 0xff, 0x0f, 0x00, 0x7e, 0xf0, 0x03,
    0x3f, 0xf8, 0xe1, 0xf1, 0x83, 0x1f, 0xfc, 0x00, 0xf8, 0xff, 0xff, 0x0f,
    0x00, 0x7e, 0xf0, 0x03, 0x3f, 0xf8, 0xe1, 0xf0, 0x83, 0x1f, 0xfc, 0x00,
    0xfc, 0xff, 0xff, 0x0f, 0x00, 0x7f, 0xf0, 0x83, 0x3f, 0xfc, 0x71, 0xf8,
    0x81, 0x1f, 0xfe, 0x00, 0xfc, 0xff, 0xff, 0x0f, 0x00, 0x3f, 0xf0, 0x83,
    0x1f, 0xfc, 0x3c, 0xf8, 0x81, 0x1f, 0x7e, 0x00, 0xfc, 0xff, 0xff, 0x1f,
    0x00, 0x3f, 0xf8, 0x83, 0x1f, 0xfc, 0x1f, 0xf8, 0xc1, 0x0f, 0x7e, 0x00,
    0xfc, 0xff, 0xff, 0x1f, 0x00, 0x3f, 0xf8, 0x81, 0x1f, 0xfc, 0x03, 0xf8,
    0xc1, 0x0f, 0x7e, 0x00, 0xec, 0xff, 0xff, 0x3f, 0x00, 0x3f, 0xf8, 0x81,
    0x1f, 0xfc, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0x00, 0xfc, 0xff, 0xff, 0x3f,
    0x00, 0x3f, 0xf8, 0xc1, 0x1f, 0xfe, 0x00, 0xfc, 0xc0, 0x0f, 0x7f, 0x00,
    0xfc, 0xff, 0xff, 0x1f, 0x80, 0x1f, 0xfc, 0xc1, 0x0f, 0xfc, 0x00, 0xfe,
    0xe0, 0x0f, 0x3f, 0x00, 0xe8, 0xff, 0xff, 0x3f, 0x80, 0x1f, 0xfc, 0xe1,
    0x0f, 0xfc, 0x00, 0xff, 0xe0, 0x8f, 0x3f, 0x00, 0x78, 0xff, 0xff, 0x1f,
    0x80, 0x3f, 0xff, 0xf1, 0x0f, 0xfc, 0x80, 0xfd, 0xf9, 0xcf, 0x3f, 0x00,
    0xf8, 0xff, 0xff, 0x1e, 0x80, 0xff, 0xfb, 0xff, 0x0f, 0xfc, 0xff, 0xfc,
    0xdf, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0xff, 0x1f, 0x00, 0xff, 0xf9, 0xff,
    0x07, 0xf8, 0x7f, 0xf8, 0xcf, 0xff, 0x1f, 0x00, 0x60, 0xff, 0xff, 0x1d,
    0x00, 0xff, 0xf1, 0xef, 0x07, 0xf8, 0x3f, 0xf8, 0x8f, 0xbf, 0x1f, 0x00,
    0xe0, 0xfb, 0xff, 0x0f, 0x00, 0x3c, 0xe0, 0xe1, 0x07, 0xe0, 0x07, 0xe0,
    0x01, 0xcf, 0x1f, 0x00, 0xc0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0xfc,
    0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x80, 0xff, 0xbf, 0x03,
    0x00, 0x00, 0x00, 0xe7, 0x07, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x1f, 0x00,
    0x00, 0xff, 0xff, 0x03, 0x00, 0x00, 0x80, 0xf1, 0x03, 0x00, 0x00, 0x00,
    0x00, 0xc6, 0x1f, 0x00, 0x00, 0xfa, 0x7f, 0x00, 0x00, 0x00, 0xc0, 0xf0,
    0x03, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x0f, 0x00, 0x00, 0xf8, 0x6f, 0x00,
    0x00, 0x00, 0xc0, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x0f, 0x00,
    0x00, 0xd8, 0x0d, 0x00, 0x00, 0x00, 0xc0, 0xf8, 0x03, 0x00, 0x00, 0x00,
    0x00, 0xe3, 0x0f, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xc0, 0xf9,
    0x01, 0x00, 0x00, 0x00, 0x00, 0xe7, 0x07, 0x00, 0x00, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
