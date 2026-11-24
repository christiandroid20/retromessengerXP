# Inventario de recursos — migración nativa

Fecha de auditoría: 2026-11-24  
Rama de migración: work/msn-whatsapp-xp-native

## Recursos conservados

| Recurso | Ubicación | Estado |
|---|---|---|
| Icono de aplicación | build/icon.png | Conservado como icono provisional. |
| Web estática | web/ | Conservada; independiente de la aplicación nativa. |
| SVG propios MSN/ICQ | docs/legacy-reference/icons.ts | Referencia para portar a Qt. |
| Catálogo de guiños | docs/legacy-reference/winks.ts | Referencia; la funcionalidad depende del futuro transporte. |
| Capturas de etapa 1 | docs/capturas/ | Evidencia histórica del prototipo, no prueba de fidelidad 1:1. |
| Créditos de sonidos | resources/sounds/CREDITOS.md | Conservados para registrar titularidad y restricciones. |

## Base nativa nueva

| Recurso | Ubicación | Estado |
|---|---|---|
| Proyecto CMake | CMakeLists.txt | Esqueleto Qt 6 / C++20. |
| Ventana principal | native/src/mainwindow.* | Shell inicial sin datos reales. |
| Gestor de skins | native/src/skinmanager.* | Selector y carga de QSS externo. |
| Skin MSN 7.5 | native/skins/msn-75.qss | Primera adaptación, no réplica completa. |
| Skin ICQ 2000b | native/skins/icq-2000b.qss | Primera adaptación, no réplica completa. |

## Fuera del runtime

- El código Electron, los preload, los selectores de WhatsApp, el CSS de WhatsApp y el renderer HTML/TypeScript se retiraron del runtime de esta rama.
- No se copió el CSS que dependía del DOM de WhatsApp a la nueva interfaz.
- Los archivos WAV históricos no se empaquetan: los créditos identifican derechos de terceros. Su presencia en el repositorio no otorga permiso de redistribución.
- Los archivos de referencia no deben considerarse módulos funcionales de la aplicación.

## Estado de funcionalidades

| Funcionalidad | Estado |
|---|---|
| Shell nativo | Esqueleto inicial |
| MSN 7.5 / ICQ 2000b | QSS preliminar |
| WLM 2009 / Yahoo! / AIM | Conceptos pendientes |
| Contactos e historial | Pendiente |
| Envío y recepción | Pendiente |
| Transportes y modo simulado | Pendiente |
| Cuentas/sesiones | Pendiente |
| Respuestas rápidas | Pendiente |
| Adjuntos y visor multimedia | Pendiente |
| Instaladores Windows/macOS/Linux | Pendiente |

## Próximas etapas

1. Completar componentes nativos de contactos y conversación usando datos ficticios.
2. Definir Messenger Core y contrato de transporte.
3. Añadir Mock Transport y pruebas de contrato.
4. Implementar almacenamiento y respuestas rápidas persistentes.
5. Migrar iconos, sonidos permitidos, cursores, fondos y otros recursos al sistema de skins.
6. Reconstruir los skins contra versiones históricas concretas.
7. Evaluar integraciones reales por separado y sin prometer compatibilidad no demostrada.
8. Añadir CI y empaquetado nativos después de que el proyecto se pueda configurar y validar localmente.
