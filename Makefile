CPPFLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/usr/include/freetype2 -I/usr/include/SDL -D_RPI_
LIBS=-L/opt/vc/lib -lbcm_host -lEGL -lGLESv2 -lfreetype -lSDL -lboost_system -lboost_filesystem -lfreeimage

include Makefile.common
