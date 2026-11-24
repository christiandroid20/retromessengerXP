# MSN-WhatsApp-XP

**Cliente de mensajería de escritorio nativo en reconstrucción.** El objetivo es crear una aplicación propia inspirada en MSN Messenger, Windows Live Messenger, ICQ, Yahoo! Messenger y AIM, sin depender de la interfaz de WhatsApp Web.

## Estado real

Esta rama inicia la sustitución del prototipo Electron por una base nativa Qt 6 + C++20.

| Área | Estado |
|---|---|
| Aplicación nativa Qt Widgets | Esqueleto inicial |
| Skin MSN Messenger 7.5 | Primera adaptación QSS; todavía no es réplica completa |
| Skin ICQ 2000b | Primera adaptación QSS; todavía no es réplica completa |
| Skins WLM 2009, Yahoo! y AIM | Conceptos pendientes de implementación |
| Carga de skins QSS externos | Base inicial |
| Lista real de contactos | Pendiente |
| Conversaciones e historial | Pendiente |
| Envío y recepción de mensajes | Pendiente |
| Cuentas y sesiones múltiples | Pendiente |
| Respuestas rápidas persistentes | Pendiente |
| Adjuntos y visor multimedia | Pendiente |
| Conectores de mensajería | Pendiente |
| Instaladores multiplataforma | Pendiente |

**Esta versión no inicia sesión en WhatsApp ni en ningún otro servicio.** La ventana muestra deliberadamente estados vacíos y controles de envío deshabilitados: todavía no existe un núcleo de mensajería ni un transporte conectado.

## Arquitectura objetivo

- **Aplicación:** C++20 y Qt 6 Widgets.
- **Interfaz:** ventanas y controles nativos, independientes de cualquier página web.
- **Messenger Core:** modelo común de cuentas, contactos, conversaciones, mensajes y eventos (pendiente).
- **Transportes:** adaptadores aislados por servicio, con un transporte simulado para desarrollo (pendiente).
- **Skins:** componentes y recursos nativos; los archivos QSS externos se podrán cargar sin recompilar.
- **Datos:** preferencias separadas de credenciales y sesiones; protección de secretos mediante mecanismos del sistema operativo.

No se incorpora Qt WebEngine ni un navegador embebido. Tampoco se mantiene Electron, electron-vite, Node.js o Chromium como runtime de escritorio.

## Skins y recursos

La primera base nativa incluye MSN 7.5 e ICQ 2000b. Sus colores, bordes y jerarquías parten del prototipo anterior, pero aún necesitan comparación con referencias históricas y reconstrucción de todos sus componentes.

Los archivos QSS externos se buscan en el directorio de datos de la aplicación, dentro de `skins/`. Los recursos gráficos propios existentes se conservan como referencia para su migración.

Los WAV históricos de MSN e ICQ tienen titulares de derechos de terceros, según `resources/sounds/CREDITOS.md`. No se empaquetan en la aplicación nativa ni se asume permiso de redistribución. La web estática de presentación bajo `web/` es independiente del cliente de escritorio.

## Requisitos de desarrollo

- CMake 3.24 o posterior.
- Qt 6.5 o posterior, módulo Widgets.
- Compilador C++20.

Cuando se autorice compilar, la configuración prevista será:

```sh
cmake -S . -B build
cmake --build build --config Release
```

**No se ha ejecutado CMake, compilación, empaquetado ni pruebas de ejecución en esta migración.**

## Reglas del proyecto

- La rama `main` no se modifica.
- La migración nativa se trabaja en `work/msn-whatsapp-xp-native`.
- No se hace force-push.
- No se eliminan recursos históricos útiles sin conservar una referencia.
- No se declara funcional una integración que no haya sido implementada y probada.
- No se integran protocolos privados o no oficiales por defecto ni se arriesgan cuentas sin evaluar antes sus condiciones y seguridad.

## Licencia

MIT para el código del proyecto, salvo los recursos que indiquen expresamente otra titularidad o licencia. La presencia de un recurso en el repositorio no implica que pueda redistribuirse.
