# project
project_name := parrydice
project = $(bin_dir)/$(project_name)

# paths
inc_dir = $(src_dir)
src_dir := src
bin_dir := bin
vnd_dir := vendor
bin_dirs := $(bin_dir) $(addprefix $(bin_dir)/, utils core)

# sources
headers := $(wildcard $(inc_dir)/*.h) $(wildcard $(inc_dir)/**/*.h)
sources := $(wildcard $(src_dir)/*.c) $(wildcard $(src_dir)/**/*.c)
objects := $(patsubst $(src_dir)/%.c,$(bin_dir)/%.o,$(sources))

# compiler flags
std = -std=c11
opt = -O0 -g
wrn = -Wall -Wextra -pedantic
inc = -I$(inc_dir) $(shell pkg-config --cflags glfw3)
def ?= -DPD_USE_DEBUG=1 -DPD_USE_EXPECT=1
libs := $(shell pkg-config --libs --static glfw3) -Wl,-rpath,/usr/local/lib -lvulkan
flags := $(strip $(std) $(opt) $(wrn) $(inc) $(def))

.PHONY: all clean deps_install deps_uninstall
all: $(project)
clean:
	-rm -rf $(bin_dir)
deps_install:
	cd $(vnd_dir)/glfw; \
	cmake -S . -B build
	make -C $(vnd_dir)/glfw/build install
deps_uninstall:
	xargs rm < $(vnd_dir)/glfw/build/install_manifest.txt

$(project): $(headers) $(sources) $(bin_dirs) $(objects)
	$(CC) $(flags) $(objects) -o $@ $(libs)

$(bin_dir)/%.o: $(src_dir)/%.c $(bin_dirs)
	$(CC) $(flags) -c $< -o $@

$(bin_dirs):
	-mkdir $@
