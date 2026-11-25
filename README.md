# MSN-WhatsApp-XP

**Cliente de mensajería de escritorio nativo en reconstrucción.** La aplicación se rehace con C++20 y Qt 6 Widgets, sin Electron, Chromium ni WhatsApp Web embebido.

## Objetivo visual

Se conservan seis identidades independientes, todas con objetivo de reconstrucción visual 1:1:

1. AIM Classic
2. AIM 7
3. Windows Live Messenger 2009 (Wave 3)
4. Yahoo! Messenger 9.0
5. MSN Messenger 7.5
6. ICQ 2000b

**Estado honesto (última actualización):** MSN 7.5 e ICQ 2000b ya tienen composiciones nativas Qt diferenciadas para lista de contactos y conversación, junto con QSS específico, tomando como referencia la implementación del fork Retro Messenger para WhatsApp (MIT). No son ya solo recoloreos de la misma ventana genérica. Sin embargo, siguen siendo una primera portabilidad estructural: no se han validado contra capturas históricas ni reproducido todos los marcos, recursos, diálogos y estados. Los otros cuatro temas continúan en la shell genérica con QSS preliminar. Ninguno se declara 1:1. Detalle y procedencia: [portabilidad de MSN/ICQ](docs/RETROMESSENGER_THEME_PORT.md) y [plan de fidelidad 1:1](docs/SKIN_1TO1_IMPLEMENTATION_PLAN.md).

WhatsApp será únicamente un servicio de conexión detrás de la interfaz propia; no habrá un séptimo skin de WhatsApp ni se copiará su interfaz.

## Estado actual

| Área | Estado |
|---|---|
| Shell de escritorio Qt | Base inicial |
| MSN 7.5 e ICQ 2000b | Composición Qt y QSS específicos; validación 1:1 pendiente |
| AIM Classic, AIM 7, WLM 2009 y Yahoo! Messenger 9 | QSS preliminares sobre shell genérica |
| Avatar cuadrado estilo MSN 7.5 | Componente nativo reutilizable |
| Foto de perfil por cuenta | Selección y guardado local en PNG |
| Perfiles múltiples | Creación y separación local por UUID |
| Carpeta persistente junto al ejecutable | Se usa si tiene permisos de escritura |
| Fallback de almacenamiento | Directorio privado de datos del usuario |
| Contactos, conversaciones e historial reales | Pendientes |
| Inicio de sesión y sesiones autenticadas de WhatsApp | Pendientes |
| Envío/recepción y adjuntos | Pendientes |

La aplicación todavía **no inicia sesión en WhatsApp**. Los perfiles locales no son sesiones autenticadas y la carpeta `session/` es una estructura preparada para el futuro conector; no contiene cookies ni tokens.

## Fotos de usuario

El avatar utiliza un formato cuadrado con marco azul, recorte de imagen y marcador de iniciales cuando todavía no hay foto. En «Archivo → Cambiar foto de perfil…» se elige la imagen del perfil seleccionado. Cada cuenta conserva su propia foto en:

```text
data/accounts/<uuid>/profile/avatar.png
```

El componente queda listo para reutilizarse en futuras filas de contactos. La lista real aún no está conectada al núcleo de mensajería.

## Almacenamiento persistente y seguridad

La app intenta guardar los datos en `data/` junto al ejecutable. Si Windows, macOS o Linux no permite escribir en esa ubicación, usa el directorio privado de datos locales de la aplicación; no fuerza permisos ni requiere ejecutar como administrador.

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
- **Transportes:** adaptadores aislados por servicio, con transporte simulado para desarrollo.
- **Skins:** perfiles visuales, ventanas, controles y catálogos de recursos independientes.
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

**No se ha ejecutado CMake, compilación, empaquetado ni pruebas de ejecución en esta etapa.**

## Reglas del proyecto

- La rama `main` no se modifica.
- No se hace force-push.
- No se eliminan recursos históricos útiles sin conservar una referencia.
- No se declara funcional una integración que no haya sido implementada y probada.
- No se integran protocolos privados o no oficiales por defecto ni se arriesgan cuentas sin evaluar antes sus condiciones y seguridad.

## Licencia

MIT para el código del proyecto, salvo los recursos que indiquen expresamente otra titularidad o licencia.
