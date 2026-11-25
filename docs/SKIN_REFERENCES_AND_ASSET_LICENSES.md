# Referencias visuales y procedencia de skins

Fecha: 2026-11-24

Este documento separa referencias de diseño, recursos propios y material de terceros. Las capturas históricas se usan para estudiar distribución, densidad, controles y paletas; no se copian binarios, ejecutables ni paquetes de recursos propietarios.

## Yahoo! Messenger 9

Referencia de diseño: versión 9.0 (2007–2009), con lista de contactos renovada, avatares más visibles, vista compacta/detallada y temas de color.

- TechCrunch, lanzamiento y cambios de Yahoo! Messenger 9: https://techcrunch.com/2007/10/29/yahoo-messenger-for-windows-gets-a-tune-up-with-v90/
- My Digital Life, paletas y skins disponibles (Purple, Icy Blue, Twinkle Pink, Green Fantasia, etc.): https://www.mydigitallife.net/change-the-skin-color-for-yahoo-messenger-9/
- Y! Messenger Plus documenta paquetes de skins históricos para versiones 6, 7 y 8, pero no se importaron esos paquetes: https://ymplus.sourceforge.net/

Implementación propia actual: `native/skins/yahoo-messenger-9.qss` y `native/resources/icons/yahoo-messenger.svg`. La paleta violeta y el globo de chat son reinterpretaciones originales; no son el logotipo oficial ni una extracción del cliente.

## Windows Live Messenger 2009

Referencia de diseño: Wave 3 / versión 14.0. Se estudiaron escenas personalizables, marcos de color para estado, favoritos, grupos, iconos de presencia y avatar a la izquierda en conversaciones.

- OldVersion, resumen de cambios de la beta WLM 2009: https://www.oldversion.com/windows/windows-live-messenger/windows-live-messenger-2009-beta/
- Softonic, explicación de escenas, marcos de presencia y ubicación de avatares: https://www.softonic.com/articulos/windows-live-messenger-2009-ya-esta-aqui
- Vito Sartori, investigación técnica sobre recursos DirectUI y extracción de assets de WLM 2009: https://vito.io/articles/2026-02-15-this-is-not-rle
- WLMOnline, reconstrucción web comunitaria basada en recursos extraídos con Messenger Plus!: https://github.com/androidWG/WLMOnline

No se copiaron los recursos extraídos de WLMOnline ni paquetes de Messenger Plus! porque el repositorio consultado no ofrece una licencia de redistribución clara para esos assets. El QSS y el icono de mariposa abstracta de esta rama son originales.

Implementación propia actual: `native/skins/wlm-2009.qss` y `native/resources/icons/wlm-2009.svg`. La apariencia es una primera interpretación del azul cristal/escenas, no una reproducción 1:1 de DirectUI.

## AIM 7

Referencia de diseño: AIM 7 como generación posterior del cliente clásico, con Buddy List, conversaciones, iconos de buddy, historial y personalización visual.

- TechSpot, ficha y características de AIM 7: https://www.techspot.com/downloads/3867-aol-instant-messenger-beta.html
- Curio, referencia histórica de densidad y lenguaje visual de Buddy List AIM clásico: https://designbycurio.com/aol-instant-messenger-1997

Implementación propia actual: `native/skins/aim-7.qss` y `native/resources/icons/aim-7.svg`. El icono de corredor es una reinterpretación geométrica propia, no un recurso oficial de AOL.

## Criterio de licencia

- Los tres QSS fueron escritos para este proyecto.
- Los tres SVG de esta carpeta son ilustraciones originales creadas para el selector de skins.
- No se redistribuyen ejecutables antiguos, DLL, archivos extraídos de instaladores, emoticonos, sonidos ni logotipos oficiales.
- Las marcas y nombres se usan únicamente para identificar el estilo de referencia; no se afirma afiliación con Yahoo, Microsoft, AOL ni sus titulares.
- Si en el futuro se incorporan recursos históricos exactos, primero debe verificarse la licencia o autorización de redistribución de cada archivo y registrar su procedencia.
