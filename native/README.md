# Cliente nativo

Este directorio contiene la aplicación de escritorio Qt 6, sin Electron ni navegador embebido.

## Requisitos

- CMake 3.24 o posterior.
- Qt 6.5 o posterior, módulo Widgets.
- Compilador C++20.

## Configuración local

No se ejecutó ninguna compilación al preparar estos cambios.

Cuando se autorice una compilación:

```sh
cmake -S . -B build
cmake --build build --config Release
```

## Fotos de usuario

- `UserAvatar` dibuja avatares cuadrados con marco azul y recorte de imagen, inspirados en MSN Messenger 7.5.
- Cada perfil local puede guardar su foto en `profile/avatar.png`.
- «Archivo → Cambiar foto de perfil…» permite seleccionar una imagen; el selector de cuenta permite cambiar entre perfiles.
- El mismo componente visual queda disponible para futuras filas de contactos. La lista real de contactos todavía no está conectada.

## Almacenamiento de cuentas

La aplicación intenta usar `data/` junto al ejecutable. Si la carpeta de instalación no permite escritura, usa el directorio privado de datos locales del usuario. No solicita permisos elevados ni intenta cambiar permisos de la carpeta de instalación.

Estructura creada:

```text
data/
  accounts.json
  accounts/
    <uuid-de-cuenta>/
      account.json
      profile/
        avatar.png
      session/
      media/
```

Cada cuenta tiene un UUID y sus propios directorios. El índice se escribe mediante `QSaveFile`; en sistemas Unix se restringen los permisos de los directorios al propietario.

**Límite actual:** se guardan los perfiles locales y la estructura preparada para cada sesión, pero todavía no hay inicio de sesión de WhatsApp ni transporte que escriba/restaure cookies, tokens o estado autenticado. No se guardan contraseñas ni secretos en JSON. La integración futura deberá usar el almacén seguro del sistema operativo para credenciales.

## Estado

- Shell nativo Qt Widgets.
- Skin QSS preliminar MSN 7.5 e ICQ 2000b.
- Avatar cuadrado reutilizable y foto de perfil persistente por perfil local.
- Creación y separación de múltiples perfiles locales.
- Directorio de datos junto a la instalación cuando es escribible; fallback privado por usuario.
- Contactos reales, historial, envío, adjuntos y transporte: pendientes.
- Persistencia de sesiones autenticadas de WhatsApp: pendiente de un conector y de un diseño seguro de credenciales.
- WLM 2009, Yahoo! Messenger y AIM: objetivos de diseño, no skins funcionales.
