# MSN-WhatsApp-XP

**Cliente de mensajería de escritorio nativo en reconstrucción.** La aplicación se está rehaciendo con C++20 y Qt 6 Widgets, sin Electron, Chromium ni WhatsApp Web embebido.

## Estado actual

| Área | Estado |
|---|---|
| Shell de escritorio Qt | Base inicial |
| Skin MSN Messenger 7.5 | Adaptación QSS preliminar |
| Skin ICQ 2000b | Adaptación QSS preliminar |\n| Skin Yahoo! Messenger 9 | QSS propio + icono SVG original |\n| Skin Windows Live Messenger 2009 | QSS propio + icono SVG original |\n| Skin AIM 7 | QSS propio + icono SVG original |
| Avatar cuadrado estilo MSN 7.5 | Componente nativo reutilizable |
| Foto de perfil por cuenta | Selección y guardado local en PNG |
| Perfiles múltiples | Creación y separación local por UUID |
| Carpeta persistente junto al ejecutable | Se usa si tiene permisos de escritura |
| Fallback de almacenamiento | Directorio privado de datos del usuario |
| Contactos, conversaciones e historial reales | Pendientes |
| Inicio de sesión y sesiones autenticadas de WhatsApp | Pendientes |
| Envío/recepción y adjuntos | Pendientes |

La aplicación todavía **no inicia sesión en WhatsApp**. Los perfiles locales no son sesiones autenticadas y la carpeta `session/` es una estructura preparada para el futuro conector; no contiene cookies ni tokens.

## Skins disponibles\n\nEl selector nativo incluye MSN Messenger 7.5, ICQ 2000b, Yahoo! Messenger 9, Windows Live Messenger 2009 y AIM 7. Los tres skins nuevos incluyen hojas QSS propias e iconos SVG originales. Son interpretaciones visuales iniciales, no clones 1:1 ni paquetes de recursos oficiales.\n\n- Yahoo! Messenger 9: paleta violeta, paneles claros y acentos púrpura.\n- Windows Live Messenger 2009: azul cristal inspirado en Wave 3, con lenguaje visual de escenas.\n- AIM 7: cromado gris oscuro, acentos AOL azul y selección amarilla.\n\nLos recursos originales y las fuentes históricas se documentan en [referencias y licencias](docs/SKIN_REFERENCES_AND_ASSET_LICENSES.md). No se importaron DLL, ejecutables, sonidos ni assets extraídos de clientes propietarios.\n\n## Fotos de usuario

El avatar utiliza un formato cuadrado con marco azul, recorte de imagen y un marcador de iniciales cuando todavía no hay foto. En «Archivo → Cambiar foto de perfil…» se elige la imagen del perfil seleccionado. Cada cuenta conserva su propia foto en:

```text
data/accounts/<uuid>/profile/avatar.png
```

El componente queda listo para reutilizarse en las futuras filas de contactos. La lista real aún no está conectada al núcleo de mensajería.

## Almacenamiento persistente y seguridad

La app intenta guardar los datos en `data/` junto al ejecutable, tal como se solicitó. Si Windows, macOS o Linux no permite escribir en esa ubicación, usa el directorio privado de datos locales de la aplicación; no fuerza permisos ni requiere ejecutar como administrador.

Estructura:

```text
data/
  accounts.json
  accounts/
    <uuid>/
      account.json
      profile/avatar.png
      session/
      media/
```

Los perfiles se separan por UUID. El índice se actualiza de forma atómica. En Unix se aplican permisos de directorio limitados al usuario propietario; en Windows se heredan los permisos de la carpeta privada cuando se usa el fallback.

No se guardan contraseñas, cookies ni tokens en archivos JSON. La persistencia real de una sesión de WhatsApp solo se podrá implementar cuando exista un transporte compatible y se integre el almacén seguro de credenciales del sistema operativo.

## Arquitectura objetivo

- **Aplicación:** C++20 y Qt 6 Widgets.
- **Interfaz:** controles nativos y skins independientes del proveedor.
- **Messenger Core:** cuentas, contactos, conversaciones, mensajes y eventos.
- **Transportes:** adaptadores aislados por servicio, con un transporte simulado para desarrollo.
- **Skins:** componentes, recursos y paquetes externos.
- **Datos:** perfiles separados y secretos protegidos por el sistema operativo.

## Requisitos de desarrollo

- CMake 3.24 o posterior.
- Qt 6.5 o posterior, módulo Widgets.
- Compilador C++20.

Cuando se autorice compilar:

```sh
cmake -S . -B build
cmake --build build --config Release
```

**No se ha ejecutado CMake, compilación, empaquetado ni pruebas de ejecución en estos cambios.**

## Reglas del proyecto

- La rama `main` no se modifica.
- No se hace force-push.
- No se eliminan recursos históricos útiles sin conservar una referencia.
- No se declara funcional una integración que no haya sido implementada y probada.
- No se integran protocolos privados o no oficiales por defecto ni se arriesgan cuentas sin evaluar antes sus condiciones y seguridad.

## Licencia

MIT para el código del proyecto, salvo los recursos que indiquen expresamente otra titularidad o licencia.
