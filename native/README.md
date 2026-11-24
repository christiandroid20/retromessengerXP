# Cliente nativo

Este directorio contiene el reemplazo nativo del prototipo Electron.

## Requisitos

- CMake 3.24 o posterior.
- Qt 6.5 o posterior, módulo Widgets.
- Compilador C++20.

## Configuración local

No se ejecutó ninguna compilación al preparar esta migración.

Cuando se autorice una compilación, la configuración prevista es:

```sh
cmake -S . -B build
cmake --build build --config Release
```

## Estado

- Shell de escritorio Qt Widgets: esqueleto inicial.
- Skins nativos MSN 7.5 e ICQ 2000b: primera adaptación QSS.
- Skins externos: carga de archivos QSS desde el directorio de datos de la aplicación.
- Contactos, historial, envío, adjuntos, cuentas y transporte: todavía no implementados.
- WLM 2009, Yahoo! Messenger y AIM: siguen siendo objetivos de diseño, no skins funcionales.
- No se utiliza Qt WebEngine, Chromium embebido, Electron ni WhatsApp Web.
