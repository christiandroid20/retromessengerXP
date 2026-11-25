# MSN-WhatsApp-XP — Arquitectura nativa

## Dirección del producto

Construir clientes de mensajería de escritorio propios, con interfaz, ventanas, listas de contactos, conversaciones, menús, diálogos, notificaciones y visores controlados por la aplicación.

Clientes objetivo:

1. MSN Messenger 7.5 — Windows XP Luna.
2. Windows Live Messenger 2009 — Windows Vista/7 Aero.
3. Yahoo! Messenger 9 — paleta púrpura y vistas de contactos.
4. ICQ 2000b — Windows 98/2000.
5. AIM 7 — Buddy List y lenguaje visual gris/amarillo.

Ninguna página web de terceros controla la interfaz. WhatsApp Web deja de ser el shell y no se incluye un navegador embebido.

## Tecnología

- C++20.
- Qt 6 Widgets.
- CMake.
- Qt Style Sheets (QSS) para los primeros skins.
- Qt Network/SQL se evaluarán cuando se definan los contratos del núcleo y el almacenamiento.
- No Electron, electron-vite, Node.js, Chromium embebido ni Qt WebEngine.

## Capas objetivo

- **Native Shell:** ventana principal, ventanas independientes de conversación, menús y navegación.
- **Skin Engine:** registro de skins, componentes, recursos, tokens y carga de paquetes externos.
- **Messenger Core:** cuentas, contactos, conversaciones, mensajes, adjuntos, presencia, eventos y errores.
- **Transport Adapter API:** contrato común para conectar, desconectar, sincronizar, enviar y recibir.
- **Mock Transport:** datos ficticios para desarrollar la aplicación sin iniciar sesión en un servicio.
- **Account/Session Manager:** separación por cuenta y protección de secretos con mecanismos del sistema operativo.
- **Media Services:** visor de imágenes/vídeo/PDF, zoom y guardado explícito.
- **Quick Replies:** plantillas persistentes compartidas por las cuentas de una misma instalación, con exportación/importación para otras computadoras.
- **Notification Service:** notificaciones nativas y activación de la conversación correspondiente.

## Sistema de skins

Un skin completo debe definir sus propios componentes, recursos, estados e interacciones para:

- Ventana principal y lista de contactos.
- Grupos, subgrupos, perfiles y estados.
- Conversaciones individuales y grupales.
- Barras de título, menús y diálogos.
- Redacción, adjuntos, progreso y errores.
- Notificaciones, sonidos y animaciones.
- Visores multimedia y preferencias.

La base Qt incluye cinco estilos QSS: MSN 7.5, ICQ 2000b, Yahoo! Messenger 9, WLM 2009 y AIM 7. Todos son adaptaciones preliminares, no reconstrucciones 1:1. Los tres skins nuevos incluyen iconos SVG originales en el selector. Las referencias y límites de redistribución se registran en `docs/SKIN_REFERENCES_AND_ASSET_LICENSES.md`.. WLM 2009, Yahoo! y AIM continúan como objetivos de diseño.

El Skin Engine carga archivos QSS externos desde el directorio de datos de la aplicación, para que los temas añadidos por el usuario no requieran recompilar el ejecutable. La evolución a paquetes de skin con manifiesto, imágenes, iconos, cursores, fuentes y sonidos queda pendiente.

## Transporte y seguridad

- La interfaz no depende de un proveedor.
- El modo simulado permitirá desarrollar listas y conversaciones sin cuenta real.
- Una API empresarial no se presenta como equivalente al acceso a cuentas personales.
- No se integran protocolos privados/no oficiales por defecto ni se elude autenticación.
- Las capacidades no disponibles se reportan a la UI; no se simula envío real.
- Los secretos no se guardan en archivos de preferencias en texto plano.
- Las sesiones de distintas cuentas deben permanecer aisladas.

## Estado real de la migración — 2026-11-24

### Ya existe en esta rama
- Proyecto CMake con objetivo Qt 6 Widgets y C++20.
- Ventana nativa inicial con panel de contactos vacío y área de conversación.
- Estados honestos sin conexión y controles de envío deshabilitados.
- Skin QSS preliminar MSN 7.5.
- Skin QSS preliminar ICQ 2000b.
- Selector de skin y carga de archivos QSS externos desde el directorio de datos.
- Recursos web estáticos conservados y separados de la aplicación de escritorio.

### Eliminado del runtime de escritorio
- Dependencias y scripts de Electron/npm.
- BrowserWindow/WebContentsView y carga de web.whatsapp.com.
- Preload e IPC de Electron.
- Selectores y CSS que modificaban el DOM de WhatsApp.
- Renderer HTML/TypeScript del prototipo.
- Workflows que ejecutaban builds de Electron.

### Implementado después de la base inicial
- Componente de avatar cuadrado con marco azul, recorte de foto e iniciales de respaldo, inspirado en MSN 7.5.
- Selector de perfil local y cambio de foto por cuenta; las imágenes se guardan en `profile/avatar.png`.
- Creación de varios perfiles locales por UUID, con índice JSON atómico y directorios independientes `profile/`, `session/` y `media/`.
- Almacenamiento junto al ejecutable cuando la carpeta es escribible; fallback al directorio privado del usuario cuando no lo es.

### Todavía no existe
- Messenger Core.
- Transport Adapter API y Mock Transport.
- Contactos reales, historial, envío/recepción y adjuntos.
- Inicio de sesión de WhatsApp y persistencia de sesiones autenticadas (pendientes).
- Ventanas reales de chat y modo combinado.
- Administrador de respuestas rápidas.
- Skins completos de WLM 2009, Yahoo! y AIM.
- Instaladores y CI nativos multiplataforma.

## Recursos reutilizables

- El icono de aplicación y la web estática se conservan.
- Los SVG propios y el catálogo de guiños se conservan en `docs/legacy-reference/` como material para portar a Qt.
- Los QSS nativos actuales son una primera interpretación de los estilos previos; no se copió CSS dependiente del DOM de WhatsApp.
- Los WAV originales de MSN/ICQ quedan fuera del paquete nativo por derechos de terceros, según su archivo de créditos.

## Criterio de aceptación

- La app inicia sin navegador ni sesión de WhatsApp.
- Ningún componente visual depende del DOM de un proveedor.
- Los skins externos se cargan sin recompilar.
- El modo simulado permite validar contactos, conversaciones y eventos.
- La app distingue claramente simulación de integración real.
- Las pruebas y empaquetados multiplataforma se incorporan después de validar el shell nativo.


## Perfiles y persistencia local — 2026-11-24

La aplicación crea perfiles locales independientes en `data/accounts/<uuid>/`. Cada perfil tiene `account.json`, `profile/avatar.png`, `session/` y `media/`. El índice `data/accounts.json` se escribe de forma atómica. Se intenta usar `data/` junto al ejecutable y, si no hay permisos de escritura, se recurre al almacenamiento privado del usuario. No se elevan privilegios ni se fuerzan ACL/permisos.

El directorio `session/` es una reserva para el futuro adaptador: hoy no hay autenticación de WhatsApp, cookies, tokens ni restauración de sesiones. Los secretos deberán integrarse con el almacén seguro del sistema operativo y nunca escribirse en JSON. El avatar de perfil sí se guarda por cuenta y el componente visual se puede reutilizar en los futuros elementos de contacto.

No se compiló ni ejecutó el proyecto para estos cambios.
