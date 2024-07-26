#!/usr/bin/python

import sys
import os
from PIL import Image
from PIL import ImageDraw
import struct

isSWAP = False
# isSWAP = True

def main():
    len_argument = len(sys.argv)
    if len_argument != 4:
        print ("")
        print ("Correct Usage:")
        print ("\tpython png2rgb565.py <png_file> <include_file> <binary_file>")
        print ("")
        sys.exit(0)

    try:
        im = Image.open(sys.argv[1])
        # print ("/* Image Width:%d Height:%d */" % (im.size[0], im.size[1]))
        if im.mode != 'RGB':
            im = im.convert('RGB')
    except Exception as e:
        print ("Fail to open png file ", sys.argv[1], ":", e)
        sys.exit(0)

    image_height = im.size[1]
    image_width = im.size[0]

    try:
        outfile = open(sys.argv[2], "w")
    except Exception as e:
        print ("Can't write the file %s: %s" % (sys.argv[2], e))
        sys.exit(0)

    try:
        binoutfile = open(sys.argv[3], "wb")
    except Exception as e:
        print ("Can't write the binary file %s: %s" % (sys.argv[3], e))
        sys.exit(0)

    print ("// Image Width: %d Height: %d" % (im.size[0], im.size[1]), file=outfile)
    print ("#include <avr/pgmspace.h> // Useful header only when working with AVR boards.\n", file=outfile)
    print ("const uint16_t %s_ST7789[] PROGMEM = {" % (sys.argv[1].split('.')[0]).split('\\')[1], file=outfile, end='')

    pix = im.load()  # load pixel array

    for h in range(image_height):
        # if (h > (59 + 4) and h < (180 + 4)): # mFrame.bin generated from 240x120 image
        for w in range(image_width):
            if ((h * 16 + w) % 16 == 0):
                print (" ", file=outfile)
                print ("\t", file=outfile, end='')

            '''

            print ("Width ", w, "\nHeight ", h, "\nImage Width ", image_width, "\nImage Height ", image_height)
            print ("Pixel ", pix[w, h])
            print ("Pixel R: ", pix[w, h][0])
            print ("Pixel G: ", pix[w, h][1])
            print ("Pixel B: ", pix[w, h][2])

            '''

            if w < image_width and h < image_height:
                R = (pix[w, h][0] >> 3)
                G = (pix[w, h][1] >> 2)
                B = (pix[w, h][2] >> 3)

                rgb = (R << 11) | (G << 5) | (B << 0)
                # if rgb == 0: rgb = 0x0020 # mFrame.bin generated from 240x120 image

                if isSWAP:
                    swap_string_low = rgb >> 8
                    swap_string_high = (rgb & 0x00FF) << 8
                    swap_string = swap_string_low | swap_string_high
                    print ("0x%04x," % swap_string, file=outfile, end='')
                    binoutfile.write(struct.pack('H', swap_string))
                else:
                    print ("0x%04x," % rgb, file=outfile, end='')
                    binoutfile.write(struct.pack('H', rgb))
            else: rgb = 0

    print ("", file=outfile)
    print ("};", file=outfile)

    outfile.close()
    binoutfile.close()

    print ("PNG file \"%s\" converted to \"%s\"" % (sys.argv[1], sys.argv[2]))

if __name__ == "__main__":
    main()