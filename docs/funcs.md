> На данный момент документация функций является еще не готовой полностью.

### `int get_hash(const char *string, char outputBuffer[65])` 

Вычисляет хеш от `string`

**Параметры:**
- `string` — исходная строка
- `outputBuffer` — буфер размера `65`, в который будет записан хеш (в строковом виде)

**Возвращает:**
- `0` — успех
- `-1` — ошибка при вычислении хеша

**Память:**
Не требует

**Пример использования:**
```C
char hash[HASH_LENGTH];

if (get_hash(string, hash) == -1) {
	...
}
```

---

### `unsigned char *pack(char *content, size_t content_len, size_t *dest_len)`

Сжимает переданный `content` с помощью `zlib`

**Параметры:**
- `content` — исходная строка для сжатия
- `content_len` — количество байт исходных данных
- `dest_len` — буфер в который будет записано количество байт сжатых данных

**Возвращает:**
- `указатель на буфер сжатых данных` — успех
- `NULL` — ошибка выделения памяти `compressed`, ошибка сжатия 

**Память:**
Вызывающий отвечает за освобождение `content` (если выделено динамически) после вызова `pack`, если `content` больше не нужен. Освобождение `compressed` обязательно после использования.

**Пример использования:**
```C
size_t file_size = 0;
char *content = malloc(file_size);
size_t compressed_len = 0;

unsigned char *compressed = pack(content, file_size, &compressed_len);
free(content);

...

free(compressed);
```

---

### `int get_file_content_size(const char *path, size_t *file_size)`

Возвращает количество байтов в файле.

**Параметры:**
- `path` — путь к файлу
- `file_size` — буфер в который будет записано количество байт содержимого файла

**Возвращает:**
- `0` — успех
- `-1` — ошибка открытия файла

**Память:**
Не требует

**Пример использования:**
```C
size_t file_size = 0;
char *path = ".";

get_file_content_size(path, &file_size);
```