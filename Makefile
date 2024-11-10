# project
project_name := parrydice
project = $(bin_dir)/$(project_name)

# paths
inc_dir = $(src_dir)
src_dir := src
bin_dir := bin
bin_dirs := $(bin_dir) $(addprefix $(bin_dir)/, utils)

# sources
headers := $(wildcard $(inc_dir)/*.h) $(wildcard $(inc_dir)/**/*.h)
sources := $(wildcard $(src_dir)/*.c) $(wildcard $(src_dir)/**/*.c)
objects := $(patsubst $(src_dir)/%.c,$(bin_dir)/%.o,$(sources))

# compiler flags
std = -std=c11
opt = -O0 -g
wrn = -Wall -Wextra -pedantic
inc = -I$(inc_dir)
def =
flags := $(strip $(std) $(opt) $(wrn) $(inc) $(def))

.PHONY: all clean
all: $(project)
clean:
	-rm -rf $(bin_dir)

$(project): $(headers) $(sources) $(bin_dirs) $(objects)
	$(CC) $(flags) $(objects) -o $@

$(bin_dir)/%.o: $(src_dir)/%.c $(bin_dirs)
	$(CC) $(flags) -c $< -o $@

$(bin_dirs):
	-mkdir $@
