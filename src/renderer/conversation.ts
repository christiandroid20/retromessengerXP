import './styles/shell.css'
import './styles/conversation.css'
import type { ConversationInfo, Settings } from '@shared/types'

const root = document.getElementById('conversation-root') as HTMLElement
const api = window.messenger
let info: ConversationInfo | null = null
let settings: Settings | null = null
let maximized = false

function render(): void {
  if (!info || !settings) return
  document.body.dataset.skin = settings.skin
  const title = settings.skin === 'icq' ? `Message Session — ${info.displayName}` : `Conversación — ${info.displayName}`
  document.title = title
  root.innerHTML = `
    <main class="conversation-window" data-skin="${settings.skin}">
      <header class="conversation-titlebar">
        <span class="conversation-app-icon">${settings.skin === 'icq' ? '✉' : '☏'}</span>
        <strong>${escapeHtml(title)}</strong>
        <nav class="conversation-window-actions">
          <button data-window="minimize" aria-label="Minimizar">_</button>
          <button data-window="maximize" aria-label="Maximizar">${maximized ? '❐' : '□'}</button>
          <button class="close" data-window="close" aria-label="Cerrar">×</button>
        </nav>
      </header>
      <section class="conversation-toolbar">
        <div class="conversation-person">
          <div class="conversation-avatar">${escapeHtml(initials(info.displayName))}</div>
          <div><b>${escapeHtml(info.displayName)}</b><small>Conversación independiente</small></div>
        </div>
        <div class="conversation-actions"><button disabled>Adjuntar</button><button disabled>Buscar</button></div>
      </section>
      <section class="conversation-history">
        <div class="conversation-empty">
          <div class="empty-glyph">✉</div>
          <h2>Ventana de conversación</h2>
          <p>Esta ventana ya funciona de forma independiente a la lista de contactos.</p>
          <p class="transport-note">La conexión del historial y el envío de mensajes se integrará mediante el núcleo de mensajería. Todavía no se envían mensajes desde esta ventana.</p>
        </div>
      </section>
      <footer class="conversation-compose">
        <div class="compose-tools"><button disabled>🙂</button><button disabled>Adjuntar archivo</button></div>
        <div class="compose-row"><textarea disabled placeholder="La escritura se habilitará al conectar el transporte de mensajes."></textarea><button disabled class="send">Enviar</button></div>
      </footer>
    </main>`
}

function initials(name: string): string {
  return name.trim().split(/\s+/).slice(0, 2).map((part) => part[0] ?? '').join('').toUpperCase() || '?'
}
function escapeHtml(value: string): string {
  return value.replace(/[&<>"']/g, (char) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' })[char]!)
}

root.addEventListener('click', (event) => {
  const button = (event.target as HTMLElement).closest<HTMLButtonElement>('[data-window]')
  if (button) api.windowControl(button.dataset.window as 'minimize' | 'maximize' | 'close')
})
api.onConversationSettings((next) => { settings = next; render() })
api.onMaximized((value) => { maximized = value; render() })

void Promise.all([api.getSettings(), api.getConversationInfo()]).then(([nextSettings, nextInfo]) => {
  settings = nextSettings
  info = nextInfo
  if (!info) {
    root.textContent = 'No se pudo recuperar la conversación solicitada.'
    return
  }
  render()
})
