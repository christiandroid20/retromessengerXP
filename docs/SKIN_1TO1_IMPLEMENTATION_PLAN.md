# Objetivo de fidelidad visual 1:1 — MSN-WhatsApp-XP

Fecha: 2026-11-24  
Rama de trabajo: `work/msn-whatsapp-xp`

## Decisión de producto

Se conservan seis identidades visuales independientes. Todas deben evolucionar de los QSS preliminares a reconstrucciones nativas fieles a una versión histórica concreta:

1. AIM Classic
2. AIM 7
3. Windows Live Messenger 2009 (Wave 3 / familia 14.0)
4. Yahoo! Messenger 9.0 para Windows
5. MSN Messenger 7.5 (línea 7.5.0324)
6. ICQ 2000b

No se desarrollará un séptimo tema que copie WhatsApp. WhatsApp será únicamente un transporte/servicio conectado detrás de la interfaz propia de MSN-WhatsApp-XP.

## Estado real al iniciar esta etapa

La aplicación es una shell Qt 6 Widgets/C++20. El selector carga seis hojas QSS globales. Esas hojas modifican colores, tipografías y bordes de una misma estructura genérica; no reproducen todavía las distintas jerarquías de ventanas, barras, listas, menús, controles, iconos ni comportamientos de cada cliente.

Por tanto, **ninguno de los seis temas puede etiquetarse actualmente como 1:1**. Los seis están en estado preliminar. No se debe presentar una captura conceptual ni una hoja QSS como prueba de fidelidad.

## Referencias históricas de partida

Las referencias sirven para identificar elementos y versiones; antes de cerrar cada diseño se debe reunir un conjunto de capturas de la ventana de contactos y de conversación de la misma versión, además de los diálogos relevantes.

### AIM Classic
- Objetivo provisional: Buddy List clásica de la familia AIM 4.x/5.x de finales de los noventa y primeros años de los 2000.
- Referencia visual: https://designbycurio.com/aol-instant-messenger-1997
- Evidencia histórica adicional: https://arstechnica.com/gaming/2017/10/remembering-aol-instant-messenger-my-first-social-network/
- La subversión exacta todavía debe fijarse con capturas verificables. No mezclar la Buddy List clásica con AIM 7.

### AIM 7
- Objetivo: AOL Instant Messenger 7.0 para Windows, no AIM Classic ni AIM 7.5.
- Referencia de versión y características: https://www.techspot.com/downloads/3867-aol-instant-messenger-beta.html
- La implementación debe respetar la Buddy List, ventanas IM, iconos de presencia, controles y distribución propios de AIM 7.0. No asumir que el estilo plano de AIM Classic es válido para AIM 7.

### Windows Live Messenger 2009
- Objetivo: Wave 3 / familia 14.0. Debe fijarse un build de referencia antes de validar detalles.
- Referencias de características y distribución: https://www.softonic.com/articulos/windows-live-messenger-2009-ya-esta-aqui
- Contexto de la versión 14.0: https://www.neowin.net/forum/topic/670792-windows-live-messenger-9-2009-1405027908-released/
- Elementos a estudiar: escenas, marco de presencia, favoritos y grupos, lista de novedades, avatares y disposición de la ventana de conversación.

### Yahoo! Messenger 9.0
- Objetivo: cliente Windows 9.0, generación lanzada en octubre de 2007.
- Referencia contemporánea: https://techcrunch.com/2007/10/29/yahoo-messenger-for-windows-gets-a-tune-up-with-v90/
- Elementos a estudiar: nueva lista de amigos, avatares, skins/temas, controles de llamadas, transferencia y ventana de conversación. No usar la interfaz web ni versiones posteriores.

### MSN Messenger 7.5
- Objetivo: línea 7.5.0324 para Windows XP.
- Referencia de versión: https://msn-messenger-xp.uptodown.com/windows
- Referencia adicional de instalación y entorno histórico: https://www.clasicosbasicos.org/software/msn-messenger
- Elementos a estudiar: cabecera/perfil, mensaje personal y estado, grupos de contactos, publicidad/espacios de contenido cuando correspondan al build, ventana de conversación, formato, emoticonos, guiños y zumbido. No sustituirlo por Windows Live Messenger.

### ICQ 2000b
- Objetivo: cliente Windows ICQ 2000b, no ICQ 99b ni 2001b.
- Referencia de evolución visual que incluye 2000b: https://weekly-geekly.imtqy.com/articles/271317/index.html
- Referencia visual/documental con elementos del programa: https://libres.uncg.edu/ir/wcu/f/Ellern_01_Making.pdf
- Elementos a estudiar: lista de contactos y grupos, iconos de estado, menú principal, acciones de usuario, paneles/servicios laterales y diálogos de mensaje. La captura de ICQ 2001b no debe usarse como si fuera 2000b.

## Arquitectura necesaria

Un QSS global no alcanza para conseguir seis reproducciones 1:1. La implementación debe separar:

- **Messenger skin profile:** identidad, versión objetivo, métricas, tipografía, paleta, recursos, nombres de controles y capacidades.
- **Contact-list window:** composición y jerarquía específica de cada mensajero.
- **Conversation window:** composición, barras, herramientas, historial y editor específicos.
- **Native asset catalog:** iconos, cursores, indicadores de presencia, fondos, marcos y sonidos; cada recurso con procedencia/licencia registrada.
- **Behavior adapters:** acciones visuales propias del mensajero traducidas a operaciones neutrales del Messenger Core (abrir conversación, cambiar estado, enviar texto, adjuntar, etc.).
- **Qt widgets:** controles nativos personalizados por perfil; evitar depender de CSS/QSS para resolver diferencias estructurales.

Los seis perfiles deben coexistir sin compartir accidentalmente métricas, recursos o reglas de distribución. Se pueden reutilizar componentes neutrales (avatar, modelo de contacto, mensaje, transporte), pero no imponer una misma ventana genérica a todos.

## Criterios de aceptación por tema

Un tema no se considera 1:1 hasta cumplir todo lo siguiente:

1. Versión y build de referencia documentados.
2. Capturas de referencia de lista de contactos, conversación y diálogos clave.
3. Dimensiones, proporciones, espaciados, tipografías, iconos y estados contrastados contra esas capturas.
4. Recursos originales solo cuando su procedencia y permiso de redistribución sean claros; de lo contrario, sustitutos originales documentados sin fingir que son assets oficiales.
5. Estructura visual propia, no solo recoloreo de la ventana genérica.
6. Estados de presencia, selección, hover, deshabilitado, menús y ventanas secundarias cubiertos.
7. Comparación visual lado a lado con una tolerancia de diferencias documentada.
8. Pruebas automatizadas de modelos/acciones sin necesitar conectar WhatsApp.
9. Revisión visual en ejecución antes de declarar el tema terminado.

## Restricciones de trabajo

- Trabajar únicamente en `work/msn-whatsapp-xp`; `main` queda intacta.
- No force-push ni operaciones destructivas.
- No compilar ni ejecutar hasta recibir autorización explícita.
- No incorporar ejecutables, DLL o paquetes de recursos propietarios sin autorización/licencia verificable.
- No afirmar que existe fidelidad 1:1 antes de superar los criterios de aceptación.
- No usar la interfaz original de WhatsApp como séptimo tema ni como base visual.


## Actualización de implementación — 2026-11-25

Se realizó una primera portabilidad estructural de MSN 7.5 e ICQ 2000b desde la referencia MIT Retro Messenger a widgets Qt:

- `MainWindow::rebuildSkinWorkspace()` crea dos jerarquías de widgets diferentes para MSN e ICQ.
- `MainWindow::buildMenus()` cambia los menús según el skin y permite cambiar de tema desde los menús históricos.
- `native/skins/msn-75.qss` y `native/skins/icq-2000b.qss` contienen reglas para los nuevos controles.
- Los controles que dependen del núcleo de mensajería continúan deshabilitados; los estados se muestran como desconectados para no simular una sesión real.

Esto corrige la limitación anterior de aplicar solo un recoloreo a una estructura genérica para esos dos temas. **No demuestra fidelidad 1:1**: la barra de título nativa del sistema sigue presente, faltan diálogos y estados completos, no se han hecho capturas comparativas y no se ha ejecutado Qt. Los otros cuatro skins no se modificaron.
