# ENVI file parser in C

Read and parse *ENVI* data files in glorious C!

- `main.cpp` shows how we can interop with **OpenCV**

- Main user functions with explanations can be found in `inc/envi-c-handling.h`:

```C
ENVI_C_Handler_t *ENVI_OpenDataFile(const char *hdrPath, const char *dataPath);
ENVI_C_Image_t *ENVI_GetCapture(ENVI_C_Handler_t *const handler, size_t captureNumber);
bool ENVI_CleanupHandler(ENVI_C_Handler_t **const handler);
bool ENVI_ReadDataFile(ENVI_C_Handler_t *const handler);
```

## Currently implemented

- [X] Parsing of F32 values
- [X] Parsing **BIL** encoded data
- [ ] Parsing **BSQ** encoded data
- [ ] Parsing **BIP** encoded data
