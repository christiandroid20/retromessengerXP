# Portabilidad de temas de Retro Messenger a Qt

Fecha: 2026-11-25  
Rama destino: `work/msn-whatsapp-xp`

## Fuente de referencia

Proyecto consultado: [Lisandro46/retroskin-msn-icq-for-wsp](https://github.com/Lisandro46/retroskin-msn-icq-for-wsp).

El repositorio fuente declara licencia MIT y atribuye el proyecto a Lisandro Arrigoni. También conserva capturas de su propia implementación en `docs/capturas/etapa1-msn.png` y `docs/capturas/etapa1-icq.png`; son referencias del fork, no capturas oficiales de los clientes históricos. La portabilidad de esta rama toma como referencia, entre otros:

- `src/renderer/skins.ts`: jerarquía de ventanas, títulos y menús para MSN 7.5 e ICQ 2000b.
- `src/renderer/styles/msn.css`: barra Luna, botones, menús, áreas de contenido y diálogos MSN.
- `src/renderer/styles/icq.css`: barra azul, controles biselados, tipografía y paneles ICQ.
- `src/renderer/styles/shell.css`: estructura general de ventana y controles.
- `src/wa/css/msn.css`, `src/wa/css/icq.css` y `src/wa/css/chrome.css`: elementos superpuestos al DOM de WhatsApp Web.

La capa `src/wa` depende del DOM de WhatsApp y **no se porta** a la aplicación Qt. Tampoco se incorpora Electron, Chromium ni WhatsApp Web. La adaptación se realiza con widgets Qt y QSS propios.

## Cambios de esta etapa

### MSN Messenger 7.5

- La lista de contactos ahora tiene una composición propia.
- Panel de perfil con avatar, cuenta local, presencia y mensaje personal de referencia.
- Encabezado de contactos y agrupación visual de conectados/no conectados.
- Ventana de conversación diferenciada, con cabecera, historial, barra de herramientas, editor y botón Enviar.
- Menús con la nomenclatura Archivo, Contactos, Acciones, Herramientas y Ayuda.
- QSS específico para paleta Luna, panel de perfil, controles, selección y superficies.

### ICQ 2000b

- Lista de contactos separada de la composición MSN.
- Panel de identidad, ICQ#, controles Main/Services, área ICQuick y estado.
- Ventana Message Session con historial, editor y botón Send.
- Menús Main, Services, Change Skin y Help.
- QSS específico para grises clásicos, biseles elevados/hundidos y barra azul.

### Compartido

- Se conserva el selector de los seis temas.
- Se reconstruye el espacio de trabajo al cambiar de skin, sin cambiar el modelo de almacenamiento de cuentas.
- Los controles que dependen de Messenger Core permanecen deshabilitados y se identifican como pendientes; no se simula envío ni conexión.
- Los cuatro temas AIM Classic, AIM 7, WLM 2009 y Yahoo! Messenger 9 no se han modificado en esta etapa.

## Procedencia y licencia

La implementación original consultada está publicada bajo MIT. Se conserva la atribución al proyecto fuente y a su autor en esta documentación. Los estilos Qt de esta rama son una adaptación nativa y no una copia directa de las hojas CSS del proyecto fuente. Los iconos SVG de esta rama continúan siendo ilustraciones propias; no se afirma que sean los recursos binarios originales de Microsoft o ICQ.

No se importaron los selectores que modifican WhatsApp Web ni se incorporaron recursos propietarios extraídos de instaladores. Los sonidos y recursos con titulares/licencias separadas mantienen sus créditos existentes.

## Estado comprobado por lectura de código

Implementado en el código fuente:

- Rutas de construcción distintas para MSN e ICQ en `MainWindow::rebuildSkinWorkspace()`.
- Menús distintos en `MainWindow::buildMenus()`.
- Reglas de estilo específicas en `native/skins/msn-75.qss` y `native/skins/icq-2000b.qss`.
- Cambio de tema que reconstruye el espacio de trabajo y conserva el identificador de cuenta seleccionado.

No comprobado todavía:

- Compilación C++/Qt.
- Ejecución en Windows, Linux o macOS.
- Apariencia real de los widgets renderizados.
- Correspondencia 1:1 con capturas históricas de MSN 7.5.0324 e ICQ 2000b.
- Comportamiento funcional de contactos, mensajes, presencia, zumbidos, emoticonos o envío.

## Pendiente para fidelidad 1:1

1. Incorporar referencias visuales verificables de la misma build histórica para cada cliente.
2. Comparar geometría, alturas, márgenes, tipografías, iconos y estados contra esas capturas.
3. Implementar marcos de ventana propios si la barra nativa del sistema impide reproducir la barra histórica.
4. Completar estados visuales y diálogos de opciones/contactos/conversación.
5. Revisar recursos y sonidos por separado, con procedencia y permisos.
6. Tras autorización explícita, compilar y realizar revisión visual en ejecución.

No se declara que los temas hayan alcanzado 99 % ni 1:1 en esta etapa.
