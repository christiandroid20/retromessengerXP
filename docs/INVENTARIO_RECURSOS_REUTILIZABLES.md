# Inventario de recursos reutilizables — MSN-WhatsApp-XP

Fecha de auditoría: 2026-11-24  
Rama auditada: work/msn-whatsapp-xp  
HEAD auditado: 092ca226e534e1e3b5a0b9bcc74188eccf89506a

Este inventario separa los recursos que pueden acelerar la reconstrucción de los que solo sirven como referencia o requieren autorización. La presencia de un archivo en el repositorio no implica que sus derechos permitan redistribuirlo.

## 1. Conservar como infraestructura

| Recurso | Ubicación | Decisión |
|---|---|---|
| Electron, electron-vite y TypeScript | package.json, electron.vite.config.ts, src/ | Conservar. Es la base de escritorio ya probada. |
| Instaladores Windows | workflows desktop-packages.yml y release.yml; electron-builder | Conservar. |
| Paquetes macOS x64/arm64 | release.yml y configuración mac | Conservar. Firma y notarización son tareas de distribución separadas. |
| Paquetes Linux AppImage/deb/rpm | release.yml y configuración linux | Conservar. |
| CI de TypeScript y build | .github/workflows/ci.yml | Conservar como barrera mínima de calidad. |
| Icono de aplicación | build/icon.png | Conservar provisionalmente como identidad del proyecto, no como arte histórico de cada cliente. |
| Preferencias y geometría | src/main/store.ts | Reutilizar para preferencias no secretas. No guardar credenciales ni secretos de sesión aquí. |
| Toast y bandeja | src/main/toast.ts, src/main/index.ts | Auditar y separar del transporte para reutilizar notificaciones. |
| Registro de ventanas | src/main/window-manager.ts | Reutilizar como punto de partida. Todavía no crea el ciclo completo de ventanas de conversación. |

## 2. Recursos gráficos y de interacción aprovechables

| Recurso | Ubicación | Estado / uso previsto |
|---|---|---|
| Dibujos SVG propios de buddy MSN | src/shared/icons.ts | Reutilizables como base y placeholders; ajustar contra capturas de la versión objetivo. |
| Flor ICQ dibujada en SVG | src/shared/icons.ts | Reutilizable como base, pendiente de comparación con la versión exacta. |
| Iconos de búsqueda, preferencias, nota, guiño y zumbido | src/shared/icons.ts | Reutilizables si encajan con la función y referencia histórica. |
| Plantillas de ventanas y diálogos | src/renderer/skins.ts | Aprovechar estructura y eventos como prototipo; no tratarlas como implementación 1:1. |
| Estilos MSN e ICQ | src/renderer/styles/msn.css, icq.css, shell.css | Extraer reglas útiles y reconstruir componentes independientes de WhatsApp. |
| Guiños | src/shared/winks.ts | Reutilizar el catálogo como interacción opcional; validar animaciones y compatibilidad con el transporte. |
| Motor de sonidos sintetizados | src/renderer/sounds.ts | Reutilizable como fallback creado por código, sin depender de WAV históricos de terceros. |
| Capturas de etapa 1 | docs/capturas/etapa1-msn.png, etapa1-icq.png | Conservar como evidencia del prototipo, no como referencia final de fidelidad. |
| Plan anterior | docs/PLAN_FIDELIDAD_RETRO.md, docs/ETAPA_1_FIDELIDAD.md | Usar como registro y checklist; actualizar objetivos dependientes de WhatsApp Web. |

## 3. Recursos con derechos o licencias por verificar

### Sonidos originales

Los WAV de resources/sounds/msn/ y resources/sounds/icq/ están identificados en resources/sounds/CREDITOS.md como sonidos originales cuyos derechos pertenecen a Microsoft y a titulares de ICQ/AOL/VK. El crédito aclara que no forman parte de la licencia MIT.

**Decisión:** conservar el registro de procedencia, pero no asumir que se pueden redistribuir en nuevos instaladores. Para paquetes públicos, usar sonidos originales creados para el proyecto, sonidos con licencia compatible o archivos que el usuario aporte localmente, salvo autorización suficiente.

### Marcas e iconografía

MSN, Windows Live Messenger, Yahoo!, ICQ y AIM son marcas de sus respectivos titulares. Los SVG de src/shared/icons.ts están descritos como dibujos propios inspirados en esos clientes, no como arte oficial.

**Decisión:** mantener avisos de no afiliación, identificar correctamente las marcas y no incorporar ejecutables, DLL, fuentes, iconos o imágenes extraídos de instaladores originales sin revisar sus licencias.

## 4. Código que no debe convertirse en el núcleo de la nueva interfaz

| Recurso | Motivo |
|---|---|
| src/preload/whatsapp.ts | Observa y modifica el DOM interno de WhatsApp Web. Es específico del sitio y frágil ante cambios. No debe controlar la UI reconstruida. |
| src/wa/selectors.ts | Selectores del DOM de WhatsApp Web. Mantener aislado solo si se conserva un adaptador opcional. |
| src/wa/css/** | Estilos que alteran la página de WhatsApp. No son base adecuada para ventanas nativas independientes. |
| createWhatsAppView() y web.whatsapp.com | Integración actual acoplada a una sola vista. No representa la arquitectura final. |
| offChrome() y overlays | Presentación del prototipo actual, no componentes del nuevo Messenger Core. |

No borrar estas piezas en bloque: primero definir la interfaz del transporte, construir un modo de desarrollo con datos ficticios y migrar la UI. El adaptador actual solo se retira o conserva después de evaluar un transporte real y sus riesgos.

## 5. Estado real de las cinco reconstrucciones

| Cliente objetivo | Estado auditado | Próximo trabajo |
|---|---|---|
| MSN Messenger 7.5 | Skin activa parcial, con marco, iconos y composición sobre WhatsApp Web | Reconstruir lista, perfil, menús, conversación, diálogos, estados y ventanas propias contra referencias exactas. |
| Windows Live Messenger 2009 | Miniatura conceptual en el selector | Crear sistema Aero propio y componentes completos; no heredar simplemente CSS de MSN 7.5. |
| Yahoo! Messenger clásico | Miniatura conceptual en el selector | Elegir versión concreta y reconstruir superficies y comportamientos. |
| ICQ 2000b | Skin activa parcial, con marco, flor e interacción sobre WhatsApp Web | Completar lista, grupos, estados, menús, Message Session, preferencias y diálogos contra referencias exactas. |
| AIM clásico | Miniatura conceptual en el selector | Elegir versión concreta y reconstruir Buddy List, IM, menús, estados y diálogos. |

## 6. Regla para lograr fidelidad 1:1

No inventar dimensiones, iconos, menús ni estados de memoria. Para cada cliente se debe congelar una versión/build, idioma y plataforma de referencia y reunir capturas de:

- Inicio de sesión y recuperación de sesión.
- Ventana principal en estado normal, maximizado y compacto.
- Lista de contactos, grupos, subgrupos y estados de presencia.
- Ventana de conversación individual y grupal.
- Menús principales y contextuales.
- Opciones, preferencias, alertas y notificaciones.
- Selector de archivos, visor multimedia y errores.
- Sonidos y secuencias de interacción cuando exista evidencia reproducible.

Cada pieza debe etiquetarse como **réplica visual**, **comportamiento reproducido**, **adaptación necesaria al transporte** o **función no disponible**. La fidelidad no se declara terminada por una captura bonita: requiere comparar estados e interacciones.

## 7. Orden de migración recomendado

1. Mantener intactos los workflows de Windows, macOS y Linux y la CI existente.
2. Crear tipos compartidos y una interfaz MessengerTransport independiente del renderer.
3. Añadir un transporte simulado con contactos, mensajes, adjuntos y eventos ficticios para desarrollar toda la interfaz sin cuentas reales.
4. Separar MessengerCore, cuentas, sesiones, notificaciones y servicios multimedia.
5. Convertir el registro de ventanas en un gestor real de ventana de contactos y ventanas independientes de conversación, con modo combinado opcional.
6. Crear un registro de skins con componentes, recursos, menús, diálogos, sonidos y tokens propios para cada mensajero.
7. Reconstruir y validar cada cliente contra su referencia congelada.
8. Evaluar por separado los transportes reales disponibles. No prometer acceso a cuentas personales mediante APIs empresariales ni usar protocolos no oficiales sin explicar riesgos.
9. Retirar el acoplamiento a WhatsApp Web solo cuando el nuevo shell y el transporte seleccionado hayan superado pruebas.

## 8. Criterios de aceptación

- La interfaz funciona completamente en modo simulado sin cargar WhatsApp Web.
- Ningún componente visual depende de selectores o clases del DOM de WhatsApp.
- Cada skin tiene componentes y comportamiento propios, no solo una hoja CSS.
- Varias ventanas de conversación pueden abrirse, enfocarse, minimizarse y cerrarse sin duplicados.
- Las sesiones de distintas cuentas están aisladas; secretos protegidos por el sistema operativo.
- CI y empaquetado siguen generando builds para Windows, macOS y Linux.
- Las pruebas distinguen simulación, integración y uso real con una cuenta.
- Los recursos redistribuidos tienen licencia compatible o autorización documentada.
