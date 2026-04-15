/*
	revis/core/include/fs.h

	author: tracehillonsign
*/

#ifndef FS_H
#define FS_H

int create_file(const char *file_name);
int create_dir(const char *path_name);
size_t get_file_size(const char *file_path);
int walk(const char *path, char *(*callback)(const char *path));

#endif