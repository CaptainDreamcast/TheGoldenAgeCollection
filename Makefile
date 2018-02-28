include Makefile.common
include $(KOS_BASE)/addons/libtari/Makefile.commondc

all: complete

actions_user:
	cp -r filesystem/assets/main/norm/sprites/dc/* filesystem/assets/main/norm/sprites

clean_user:
