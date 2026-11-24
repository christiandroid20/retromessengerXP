# MSN-WhatsApp-XP — Architecture and implementation plan

## Product direction

Recreate the desktop-messenger experience of the classic clients, rather than merely applying a retro stylesheet to WhatsApp Web. The application shell owns the windows, navigation, menus, themes, notifications, media viewers and preferences. WhatsApp Web remains an isolated messaging surface hosted by Electron.

## Non-negotiable window behavior

### Independent mode (default)
- A dedicated contacts/conversation-list window is the home window.
- Each open conversation is a separate native application window.
- Multiple conversation windows may remain open simultaneously.
- Every independent conversation window displays the blue/title bar labelled **Conversación**, plus the contact's name and picture.

### Combined mode (optional)
- Contacts and the selected conversation appear together in one window.
- The **Conversación** title bar is not displayed in this mode.

These rules apply to every skin and must not be implemented as skin-specific exceptions.

## Skin system

The skin identifier selects a complete presentation package, not just a stylesheet:
1. MSN Messenger 7.x (classic Windows XP)
2. Windows Live Messenger 2009 (Aero)
3. Yahoo! Messenger classic
4. ICQ 2000b
5. AIM classic

Each skin must provide coordinated assets and styling for the contacts list, conversation windows, title bars, menus, context menus, attachment menus, login, notifications, sounds and media viewers. A visual skin chooser must show representative previews before applying a skin.

Color customization is a separate layer from skin selection. Users can adjust supported window/title/panel/border/selection/button colors and save named palettes without modifying the underlying skin.

## Proposed application layers

- **Window manager (main process):** creates, tracks, focuses and restores the contacts window and one window per conversation; enforces independent/combined mode.
- **Messenger shell (renderer):** owns navigation, contact rows, chat chrome, menus, settings and visual skin previews.
- **WhatsApp adapter (isolated WebContentsView + preload):** observes supported WhatsApp Web UI state and performs narrowly scoped user actions. It must not own the application's window layout.
- **Skin registry:** maps a stable skin ID to tokens, assets, component styles, menu definitions, sound mappings and preview data.
- **Palette store:** validates and persists user color overrides independently from the selected skin.
- **Account/session manager:** isolates each linked WhatsApp account and its persistent partition. Session storage must be protected and must not be written as plaintext secrets.
- **Media services:** provide explicit Save As flows, a dedicated image/video viewer and a PDF viewer with remembered page state.
- **Notification service:** emits one themed notification surface and routes activation to the corresponding conversation window.

## Migration strategy

The current upstream application has a single BrowserWindow and a single WebContentsView using the persistent partition persist:whatsapp. The existing MSN/ICQ CSS, menus, sound handling, toast implementation, settings store and WhatsApp selectors are useful reference/integration points, but the single-window ownership model is not the target architecture.

Refactor incrementally:
1. Keep main unchanged; all project work happens on work/msn-whatsapp-xp.
2. Extract and test the WhatsApp adapter boundary before adding more windows.
3. Introduce a window registry with stable conversation keys and explicit mode handling.
4. Move contacts and conversation chrome into native application renderers; do not create a browser-wrapper window per chat.
5. Add the skin registry and visual preview chooser.
6. Add independent palettes, account isolation and media viewers.
7. Validate Windows, Linux and macOS builds in CI and perform real-device/manual WhatsApp login tests.

## Security and compatibility constraints

- Keep Electron context isolation enabled. Renderer code must access privileged operations only through narrow preload APIs.
- Validate IPC sender identity and payload shape for every state-changing operation.
- Do not scrape private WhatsApp databases, extract encryption keys or bypass linked-device authentication.
- Do not assume the WhatsApp Web DOM is stable; centralize selectors and handle missing elements safely.
- Do not store account credentials or session secrets in plaintext.
- Do not silently place writable user data inside protected install directories; provide an explicit portable or user-selected data-location strategy.
- Preserve microphone/camera and necessary clipboard behavior while preventing duplicate generic OS notifications.
- Keep original upstream history and functionality available; avoid force-pushes and destructive branch operations.

## Current baseline note

This document records the intended architecture and migration constraints. It does not claim that multi-window mode, five complete skins, multi-account support or media viewers are already implemented. Each capability must be delivered and tested in its own change.