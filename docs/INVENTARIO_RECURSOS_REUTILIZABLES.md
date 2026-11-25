# Inventario de recursos — migración nativa

Fecha de auditoría: 2026-11-24  
Rama de trabajo: `work/msn-whatsapp-xp`

## Recursos conservados

| Recurso | Ubicación | Estado |
|---|---|---|
| Icono de aplicación | `build/icon.png` | Icono provisional. |
| Web estática | `web/` | Conservada; independiente de la aplicación Qt. |
| SVG propios MSN/ICQ | `docs/legacy-reference/icons.ts` | Referencia para portar a Qt. |
| Catálogo de guiños | `docs/legacy-reference/winks.ts` | Referencia; la funcionalidad depende del futuro transporte. |
| Capturas de etapa 1 | `docs/capturas/` | Evidencia histórica del prototipo; no prueban fidelidad 1:1. |
| Créditos de sonidos | `resources/sounds/CREDITOS.md` | Conservados para registrar titularidad y restricciones. |

## Base nativa

| Recurso | Ubicación | Estado |
|---|---|---|
| Proyecto CMake | `CMakeLists.txt` | Esqueleto Qt 6 / C++20. |
| Ventana principal | `native/src/mainwindow.*` | Shell inicial sin datos reales. |
| Gestor de skins | `native/src/skinmanager.*` | Selector y carga de QSS externo. |
| Skin MSN 7.5 | `native/skins/msn-75.qss` | QSS preliminar; réplica completa pendiente. |
| Skin ICQ 2000b | `native/skins/icq-2000b.qss` | QSS preliminar; réplica completa pendiente. |
| Skin Yahoo! Messenger 9 | `native/skins/yahoo-messenger-9.qss` | QSS preliminar; réplica completa pendiente. |
| Skin WLM 2009 | `native/skins/wlm-2009.qss` | QSS preliminar; réplica completa pendiente. |
| Skin AIM Classic | `native/skins/aim-classic.qss` | QSS preliminar; réplica completa pendiente. |
| Skin AIM 7 | `native/skins/aim-7.qss` | QSS preliminar; réplica completa pendiente. |
| Iconos originales | `native/resources/icons/*.svg` | SVG propios para Yahoo!, WLM 2009 y AIM; no son logotipos oficiales. |

## Referencias y licencias

- [Procedencia y licencias](SKIN_REFERENCES_AND_ASSET_LICENSES.md)
- [Objetivos y criterios de fidelidad 1:1](SKIN_1TO1_IMPLEMENTATION_PLAN.md)

No se copiaron DLL, ejecutables, paquetes extraídos ni assets de clientes propietarios sin una licencia de redistribución clara.

## Fuera del runtime

- El código Electron, los preload, los selectores de WhatsApp, el CSS de WhatsApp y el renderer HTML/TypeScript se retiraron del runtime de esta rama.
- No se copió el CSS que dependía del DOM de WhatsApp a la nueva interfaz.
- Los archivos WAV históricos no se empaquetan: los créditos identifican derechos de terceros. Su presencia en el repositorio no otorga permiso de redistribución.
- Los archivos de referencia no deben considerarse módulos funcionales de la aplicación.

## Estado de funcionalidades

| Funcionalidad | Estado |
|---|---|
| Shell nativo | Esqueleto inicial |
| Seis skins | QSS preliminares; estructura visual 1:1 pendiente |
| Contactos e historial | Pendiente |
| Envío y recepción | Pendiente |
| Transportes y modo simulado | Pendiente |
| Cuentas/sesiones autenticadas | Pendiente |
| Respuestas rápidas | Pendiente |
| Adjuntos y visor multimedia | Pendiente |
| Instaladores Windows/macOS/Linux | Pendiente |

## Próximas etapas

1. Reunir capturas de referencia verificables de cada versión/build, tanto de lista de contactos como de conversación y diálogos.
2. Definir perfiles de skin con métricas, tipografía, recursos y capacidades específicas.
3. Crear ventanas de lista de contactos y conversación específicas por mensajero, reutilizando solo modelos y controles neutrales.
4. Migrar iconos, cursores, fondos y otros recursos con procedencia/licencia clara.
5. Implementar Messenger Core y contrato de transporte independiente de la apariencia.
6. Añadir transporte simulado y pruebas de contrato.
7. Implementar almacenamiento y respuestas rápidas persistentes.
8. Realizar comparación visual en ejecución y documentar las diferencias antes de declarar cada tema 1:1.
9. Añadir CI y empaquetado nativos después de autorizar y completar validación local.
