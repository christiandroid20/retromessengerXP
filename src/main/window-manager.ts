import { BrowserWindow } from 'electron'

/**
 * Tracks native top-level windows by stable application identity.
 *
 * The contacts window is unique. Conversation windows are keyed by a stable
 * WhatsApp chat identifier, never by a display name (names can change or clash).
 * Every BrowserWindow owns its own WebContentsView; a view must never be moved
 * between BrowserWindows.
 */
export class NativeWindowManager {
  private contacts?: BrowserWindow
  private readonly conversations = new Map<string, BrowserWindow>()

  openContacts(create: () => BrowserWindow): BrowserWindow {
    const current = this.getContacts()
    if (current) {
      this.focus(current)
      return current
    }
    return this.registerContacts(create())
  }

  registerContacts(window: BrowserWindow): BrowserWindow {
    this.contacts = window
    window.once('closed', () => {
      if (this.contacts === window) this.contacts = undefined
    })
    return window
  }

  getContacts(): BrowserWindow | undefined {
    if (this.contacts?.isDestroyed()) this.contacts = undefined
    return this.contacts
  }

  openConversation(chatId: string, create: () => BrowserWindow): BrowserWindow {
    const current = this.getConversation(chatId)
    if (current) {
      this.focus(current)
      return current
    }
    return this.registerConversation(chatId, create())
  }

  getOpenConversationIds(): string[] {
    const ids: string[] = []
    for (const [key, window] of this.conversations) {
      if (window.isDestroyed()) this.conversations.delete(key)
      else ids.push(key.slice('conversation:'.length))
    }
    return ids
  }

  registerConversation(chatId: string, window: BrowserWindow): BrowserWindow {
    const key = this.conversationKey(chatId)
    const previous = this.getConversation(chatId)
    if (previous && previous !== window) {
      throw new Error(`A conversation window is already registered for ${chatId}`)
    }
    this.conversations.set(key, window)
    window.once('closed', () => {
      if (this.conversations.get(key) === window) this.conversations.delete(key)
    })
    return window
  }

  getConversation(chatId: string): BrowserWindow | undefined {
    const key = this.conversationKey(chatId)
    const window = this.conversations.get(key)
    if (window?.isDestroyed()) {
      this.conversations.delete(key)
      return undefined
    }
    return window
  }

  focus(window: BrowserWindow): void {
    if (window.isDestroyed()) return
    if (window.isMinimized()) window.restore()
    window.show()
    window.focus()
  }

  closeConversation(chatId: string): void {
    this.getConversation(chatId)?.close()
  }

  closeAllConversations(): void {
    for (const window of this.conversations.values()) {
      if (!window.isDestroyed()) window.close()
    }
    this.conversations.clear()
  }

  private conversationKey(chatId: string): string {
    const normalized = chatId.trim()
    if (!normalized || normalized.length > 256) {
      throw new Error('A stable WhatsApp chat identifier is required')
    }
    return `conversation:${normalized}`
  }
}
