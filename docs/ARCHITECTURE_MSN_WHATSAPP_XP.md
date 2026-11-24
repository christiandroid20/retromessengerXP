# MSN-WhatsApp-XP — Arquitectura objetivo y plan de reconstrucción

## Dirección del producto

Reconstruir cinco clientes de mensajería clásicos como aplicaciones de escritorio propias, con fidelidad visual y de interacción respecto de versiones concretas:

1. MSN Messenger 7.5 — Windows XP Luna.
2. Windows Live Messenger 2009 — Windows Vista/7 Aero.
3. Yahoo! Messenger clásico — versión por congelar.
4. ICQ 2000b — Windows 98/2000.
5. AIM clásico — versión por congelar.

No se trata de vestir WhatsApp Web con CSS. La aplicación es dueña de la interfaz, las ventanas, la navegación, las listas de contactos, las conversaciones, los menús, los diálogos, las notificaciones, los visores y las preferencias. Ninguna página web de terceros debe controlar el diseño ni la arquitectura del producto.

El transporte de mensajes se conecta por adaptadores intercambiables. WhatsApp solo podrá ser uno de ellos si existe una integración viable, segura y compatible con el tipo de cuenta que se pretende usar. El modo simulado permitirá desarrollar y probar la aplicación completa sin iniciar sesión en ningún servicio.

## Reglas de ventanas

### Modo independiente — predeterminado

- Una ventana propia para contactos y lista de conversaciones.
- Cada conversación abierta tiene su propia ventana nativa.
- Se permiten varias conversaciones abiertas al mismo tiempo.
- Cada ventana de chat muestra una barra azul titulada **Conversación**, junto al nombre y la imagen del contacto, en los skins donde así se haya definido el requisito visual.
- Reabrir una conversación existente enfoca su ventana en vez de crear un duplicado.

### Modo combinado — opcional

- Contactos y conversación seleccionada dentro de una ventana.
- No se muestra la barra independiente **Conversación**.
- Cambiar de modo no debe perder borradores, selección ni estado de lectura.

Estas reglas son compartidas por todos los skins, aunque cada cliente conserve su geometría y comportamiento histórico.

## Sistema de skins completos

Un skin no es un tema CSS. Cada identidad visual debe definir componentes, recursos, jerarquía, estados y comportamiento propios para:

- Inicio de sesión, vinculación y recuperación.
- Ventana principal, lista de contactos, grupos y subgrupos.
- Perfiles, avatares, mensajes personales y estados de presencia.
- Ventanas de conversación individual y grupal.
- Barras de título, menús, menús contextuales y diálogos.
- Redacción, envío, adjuntos, progreso y errores.
- Notificaciones, sonidos y animaciones.
- Visores de imagen, vídeo y PDF.
- Preferencias, accesibilidad, tamaños y escalado.

El selector de skins muestra vistas previas reales de cada sistema, no maquetas genéricas reutilizadas. Las paletas personalizadas se guardan en una capa independiente del skin.

## Capas de la aplicación

- **Native Window Manager:** crea, registra, enfoca, restaura y cierra la ventana de contactos y las ventanas de conversación. Aplica modo independiente/combinado.
- **Messenger Shell:** interfaz propia y navegación; no importa selectores, clases ni DOM de un servicio externo.
- **Messenger Core:** modelo unificado de cuentas, contactos, conversaciones, mensajes, eventos, adjuntos, presencia y errores.
- **Transport Adapter API:** contrato común para conectar, desconectar, consultar capacidades, cargar contactos/conversaciones, enviar mensajes y recibir eventos.
- **Transport adapters:** implementaciones aisladas por servicio. Ningún adaptador define el aspecto de la aplicación.
- **Mock Transport:** transporte local de desarrollo con datos ficticios, eventos, mensajes y adjuntos de prueba.
- **Skin Registry:** componentes, recursos, tokens, menús, diálogos, sonidos, iconos y reglas de interacción por cliente.
- **Account/Session Manager:** cuentas aisladas y almacenamiento seguro de sesiones, sin secretos en texto plano.
- **Media Services:** visor amplio de imágenes y vídeo, zoom sin deformación, Save As explícito y visor PDF con memoria de página.
- **Notification Service:** notificaciones dibujadas por la aplicación, sin duplicados del sistema operativo, con activación de la conversación correspondiente.
- **Preferences/Palette Store:** preferencias, geometría, paletas y configuración no secreta.

## Integraciones y límites

- No asumir que WhatsApp Web será el motor permanente.
- No presentar WhatsApp Cloud API como acceso equivalente a cuentas personales: es una API empresarial.
- No integrar protocolos no oficiales por defecto ni arriesgar números/cuentas sin evaluar sus términos, mantenimiento, seguridad y posibilidad de bloqueo.
- No raspar bases privadas, extraer claves, interceptar protocolos privados ni eludir autenticación de dispositivos vinculados.
- Si un transporte no ofrece una función, el Core informa su capacidad y la UI adapta esa acción; no se inventa compatibilidad.
- El Mock Transport no representa una cuenta real ni valida compatibilidad del proveedor.

## Recursos reutilizables

El inventario detallado está en INVENTARIO_RECURSOS_REUTILIZABLES.md. Como regla general:

- Conservar Electron, electron-vite, TypeScript, workflows de CI y empaquetado Windows/macOS/Linux.
- Reutilizar store para preferencias no secretas, la base de window-manager, toast/bandeja y componentes auxiliares después de desacoplarlos.
- Reutilizar como punto de partida los SVG propios, plantillas MSN/ICQ, guiños y motor de sonidos sintetizados; comparar cada recurso contra referencias exactas.
- Conservar las capturas de etapa 1 como evidencia del prototipo, no como prueba de fidelidad 1:1.
- Los WAV históricos de MSN e ICQ tienen derechos de terceros según el archivo de créditos. No asumir que pueden redistribuirse en nuevos paquetes sin autorización.
- Mantener aislados el preload, selectores y CSS que manipulan WhatsApp Web. No convertirlos en base de la nueva interfaz.

## Referencias y fidelidad 1:1

Antes de reconstruir cada cliente se debe congelar versión/build, idioma, plataforma y tema. Reunir referencias de todas las superficies: login, lista, estados, menús, conversación individual/grupal, opciones, notificaciones, adjuntos y visores.

Cada elemento se etiqueta como:
- **Réplica visual:** geometría, tipografía, iconografía y composición comparadas.
- **Comportamiento reproducido:** interacción y estados probados.
- **Adaptación al transporte:** diferencia necesaria porque el servicio conectado no ofrece el comportamiento histórico.
- **No disponible:** función que no puede ofrecer el transporte elegido.

No declarar una skin 1:1 a partir de una sola captura o de una miniatura de galería. Se necesita comparación de estados e interacciones contra la referencia elegida.

## Plan de migración incremental

1. Preservar workflows y empaquetado actuales. No modificar main ni borrar el adaptador actual en bloque.
2. Añadir tipos compartidos del dominio de mensajería y el contrato Transport Adapter.
3. Crear Mock Transport y pruebas de contrato; permitir que la aplicación arranque sin cargar WhatsApp Web.
4. Crear Messenger Core y mover el renderer de WaState a estados neutrales de conexión/mensajería.
5. Desacoplar componentes MSN/ICQ del DOM de WhatsApp y hacer que el shell renderice contactos y conversaciones propios.
6. Completar Native Window Manager para contactos + múltiples ventanas de chat y modo combinado.
7. Implementar el registro de skins con componentes, recursos, menús, diálogos y comportamientos separados.
8. Reconstruir cada cliente contra su versión congelada, uno por uno, con pruebas visuales y de interacción.
9. Implementar cuentas/sesiones aisladas, servicios multimedia, preferencias y notificaciones.
10. Evaluar transportes reales por separado y retirar la integración antigua solo después de que el reemplazo supere pruebas.
11. Mantener CI y paquetes de Windows, macOS y Linux verdes en cada etapa.

## Seguridad y compatibilidad

- Mantener contextIsolation y APIs preload estrechas.
- Validar emisor IPC y payload de cada operación con efectos.
- Proteger sesiones mediante mecanismos seguros del sistema operativo cuando estén disponibles.
- No escribir secretos en settings.json ni dentro de la carpeta de instalación.
- Considerar diferencias de permisos y rutas de datos entre Windows, macOS y Linux.
- No forzar notificaciones OS duplicadas; cerrar una ventana debe ocultarla si así lo define la preferencia, mientras que salir termina el proceso.
- Mantener historial upstream y evitar force-push o cambios destructivos.

## Estado real auditado — 2026-11-24

### Infraestructura conservable
- Electron/electron-vite/TypeScript y configuración de electron-builder.
- Workflows de CI y empaquetado para Windows, macOS y Linux.
- Preferencias locales, sonidos personales, toast/bandeja y base de registro de ventanas.
- SVG propios de MSN/ICQ, plantillas y estilos parciales, guiños, sintetizador y capturas de la primera etapa.

### Prototipo existente, no producto final
- La app todavía carga WhatsApp Web en un WebContentsView.
- Existe una sola BrowserWindow principal.
- El window manager registra la ventana principal y mantiene claves de conversación, pero aún no crea ventanas de chat.
- MSN e ICQ son skins parciales sobre la página de WhatsApp; no son aplicaciones reconstruidas de extremo a extremo.
- WLM 2009, Yahoo! Messenger y AIM solo aparecen como miniaturas conceptuales en el selector.
- No hay aún Messenger Core, contrato de transporte neutral ni Mock Transport completo.
- Multi-cuenta, modo combinado, paletas completas y visores multimedia/PDF dedicados siguen pendientes.

### Criterio de honestidad
Una compilación exitosa demuestra que el empaquetado funciona, no que las cinco reconstrucciones estén terminadas. Una vista previa no es un skin completo; una clase que registra ventanas no es multi-ventana funcional; un Mock Transport no es una integración real. Cada etapa se reportará con pruebas concretas.
