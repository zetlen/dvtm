/* valid curses attributes are listed below they can be ORed
 *
 * A_NORMAL        Normal display (no highlight)
 * A_STANDOUT      Best highlighting mode of the terminal.
 * A_UNDERLINE     Underlining
 * A_REVERSE       Reverse video
 * A_BLINK         Blinking
 * A_DIM           Half bright
 * A_BOLD          Extra bright or bold
 * A_PROTECT       Protected mode
 * A_INVIS         Invisible or blank mode
 */

enum {
	DEFAULT,
	HIGH,
  LOW
};

static Color colors[] = {
	[DEFAULT] = { .fg = -1,           .bg = -1, .fg256 = -1,  .bg256 = -1, },
	[HIGH]    = { .fg = COLOR_YELLOW, .bg = -1, .fg256 = 246, .bg256 = -1, },
	[LOW]     = { .fg = COLOR_WHITE,  .bg = -1, .fg256 = 249, .bg256 = -1, },
};

#define COLOR(c)        COLOR_PAIR(colors[c].pair)
/* curses attributes for the currently focused window */
#define SELECTED_ATTR   (COLOR(HIGH) | A_NORMAL)
/* curses attributes for normal (not selected) windows */
#define NORMAL_ATTR     (COLOR(LOW) | A_DIM)
/* curses attributes for a window with pending urgent flag */
#define URGENT_ATTR     NORMAL_ATTR
/* curses attributes for the status bar */
#define BAR_ATTR        (COLOR(HIGH) | A_NORMAL)
/* characters for beginning and end of status bar message */
#define BAR_BEGIN       ' '
#define BAR_END         ' '
/* status bar (command line option -s) position */
#define BAR_POS         BAR_TOP /* BAR_BOTTOM, BAR_OFF */
/* whether status bar should be hidden if only one client exists */
#define BAR_AUTOHIDE    false
/* master width factor [0.1 .. 0.9] */
#define MFACT 0.5
/* number of clients in master area */
#define NMASTER 1
/* scroll back buffer size in lines */
#define SCROLL_HISTORY 9999
/* printf format string for the tag in the status bar */
#define TAG_SYMBOL   ""
/* curses attributes for the currently selected tags */
#define TAG_SEL      (COLOR(HIGH) | A_BOLD)
/* curses attributes for not selected tags which contain no windows */
#define TAG_NORMAL   (COLOR(HIGH) | A_NORMAL)
/* curses attributes for not selected tags which contain windows */
#define TAG_OCCUPIED (COLOR(HIGH) | A_NORMAL)
/* curses attributes for not selected tags which with urgent windows */
#define TAG_URGENT (COLOR(HIGH) | A_NORMAL | A_BLINK)

#define LSYM_TILERIGHT  " \u2520\u2500 "
#define LSYM_GRID       "\u2500\u253C\u2500 "
#define LSYM_BSTACK     "\u2501\u252F\u2501 "
#define LSYM_FULL       "\u203E\u203E\u203E"

const char tags[][8] = { "A", "B" };

#include "tile.c"
#include "grid.c"
#include "bstack.c"
#include "fullscreen.c"

/* by default the first layout entry is used */
static Layout layouts[] = {
	{ LSYM_TILERIGHT, tile },
	{ LSYM_GRID, grid },
	{ LSYM_BSTACK, bstack },
	{ LSYM_FULL, fullscreen },
};

#define MOD  CTRL('a')
#define TAGKEYS(KEY,TAG) \
	{ { MOD, 'v', KEY,     }, { view,           { tags[TAG] }               } }, \
	{ { MOD, 't', KEY,     }, { tag,            { tags[TAG] }               } }, \
	{ { MOD, 'V', KEY,     }, { toggleview,     { tags[TAG] }               } }, \
	{ { MOD, 'T', KEY,     }, { toggletag,      { tags[TAG] }               } },

/* you can specifiy at most 3 arguments */
static KeyBinding bindings[] = {
	{ { MOD, 'C',          }, { create,         { NULL }                    } },
	{ { MOD, 'c',          }, { create,         { NULL, NULL, "$CWD" }      } },
	{ { MOD, 'x', 'x',     }, { killclient,     { NULL }                    } },
// { { MOD, 'j',          }, { focusnext,      { NULL }                    } },
	{ { MOD, 'j',          }, { focusdown,      { NULL }                    } },
	{ { MOD, 'k',          }, { focusup,        { NULL }                    } },
	{ { MOD, 'h',          }, { focusleft,      { NULL }                    } },
	{ { MOD, 'l',          }, { focusright,     { NULL }                    } },
	{ { MOD, 'k',          }, { focusprev,      { NULL }                    } },
	{ { MOD, 'f',          }, { setlayout,      { LSYM_TILERIGHT }          } },
	{ { MOD, 'g',          }, { setlayout,      { LSYM_GRID }               } },
	{ { MOD, 'b',          }, { setlayout,      { LSYM_BSTACK }                   } },
	{ { MOD, 'm',          }, { setlayout,      { LSYM_FULL }                   } },
	{ { MOD, ' ',          }, { setlayout,      { NULL }                    } },
	{ { MOD, 'i',          }, { incnmaster,     { "+1" }                    } },
	{ { MOD, 'd',          }, { incnmaster,     { "-1" }                    } },
	{ { MOD, 'H',          }, { setmfact,       { "-0.05" }                 } },
	{ { MOD, 'L',          }, { setmfact,       { "+0.05" }                 } },
	{ { MOD, '.',          }, { toggleminimize, { NULL }                    } },
	{ { MOD, 's',          }, { togglebar,      { NULL }                    } },
	{ { MOD, 'S',          }, { togglebarpos,   { NULL }                    } },
	{ { MOD, 'M',          }, { togglemouse,    { NULL }                    } },
	{ { MOD, '\n',         }, { zoom ,          { NULL }                    } },
	{ { MOD, '\r',         }, { zoom ,          { NULL }                    } },
	{ { MOD, '1',          }, { focusn,         { "1" }                     } },
	{ { MOD, '2',          }, { focusn,         { "2" }                     } },
	{ { MOD, '3',          }, { focusn,         { "3" }                     } },
	{ { MOD, '4',          }, { focusn,         { "4" }                     } },
	{ { MOD, '5',          }, { focusn,         { "5" }                     } },
	{ { MOD, '6',          }, { focusn,         { "6" }                     } },
	{ { MOD, '7',          }, { focusn,         { "7" }                     } },
	{ { MOD, '8',          }, { focusn,         { "8" }                     } },
	{ { MOD, '9',          }, { focusn,         { "9" }                     } },
	{ { MOD, '\t',         }, { focuslast,      { NULL }                    } },
	{ { MOD, 'q', 'q',     }, { quit,           { NULL }                    } },
	{ { MOD, 'a',          }, { togglerunall,   { NULL }                    } },
	{ { MOD, CTRL('L'),    }, { redraw,         { NULL }                    } },
	{ { MOD, 'r',          }, { redraw,         { NULL }                    } },
	{ { MOD, 'E',          }, { copymode,       { "dvtm-editor" }           } },
	{ { MOD, 'e',          }, { copymode,       { "dvtm-pager" }            } },
	{ { MOD, '/',          }, { copymode,       { "dvtm-pager", "/" }       } },
	{ { MOD, 'p',          }, { paste,          { NULL }                    } },
	{ { MOD, KEY_PPAGE,    }, { scrollback,     { "-1" }                    } },
	{ { MOD, KEY_NPAGE,    }, { scrollback,     { "1"  }                    } },
	{ { MOD, '?',          }, { create,         { "man dvtm", "dvtm help" } } },
	{ { MOD, MOD,          }, { send,           { (const char []){MOD, 0} } } },
	{ { KEY_SPREVIOUS,     }, { scrollback,     { "-1" }                    } },
	{ { KEY_SNEXT,         }, { scrollback,     { "1"  }                    } },
	// { { MOD, '0',          }, { view,           { NULL }                    } },
	// { { MOD, 'v', '0'      }, { view,           { NULL }                    } },
	// { { MOD, 'v', '\t',    }, { viewprevtag,    { NULL }                    } },
	// { { MOD, 't', '0'      }, { tag,            { NULL }                    } },
	// TAGKEYS( 'a',                              0)
	// TAGKEYS( 'b',                              1)
};

static const ColorRule colorrules[] = {
	{ "", A_NORMAL, &colors[DEFAULT] }, /* default */
};

/* possible values for the mouse buttons are listed below:
 *
 * BUTTON1_PRESSED          mouse button 1 down
 * BUTTON1_RELEASED         mouse button 1 up
 * BUTTON1_CLICKED          mouse button 1 clicked
 * BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
 * BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
 * BUTTON2_PRESSED          mouse button 2 down
 * BUTTON2_RELEASED         mouse button 2 up
 * BUTTON2_CLICKED          mouse button 2 clicked
 * BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
 * BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
 * BUTTON3_PRESSED          mouse button 3 down
 * BUTTON3_RELEASED         mouse button 3 up
 * BUTTON3_CLICKED          mouse button 3 clicked
 * BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
 * BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
 * BUTTON4_PRESSED          mouse button 4 down
 * BUTTON4_RELEASED         mouse button 4 up
 * BUTTON4_CLICKED          mouse button 4 clicked
 * BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
 * BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
 * BUTTON_SHIFT             shift was down during button state change
 * BUTTON_CTRL              control was down during button state change
 * BUTTON_ALT               alt was down during button state change
 * ALL_MOUSE_EVENTS         report all button state changes
 * REPORT_MOUSE_POSITION    report mouse movement
 */

#ifdef NCURSES_MOUSE_VERSION
# define CONFIG_MOUSE /* compile in mouse support if we build against ncurses */
#endif

#define ENABLE_MOUSE true /* whether to enable mouse events by default */

#ifdef CONFIG_MOUSE
static Button buttons[] = {
	{ BUTTON1_CLICKED,        { mouse_focus,      { NULL  } } },
	{ BUTTON1_DOUBLE_CLICKED, { mouse_fullscreen, { LSYM_FULL } } },
	{ BUTTON2_CLICKED,        { mouse_zoom,       { NULL  } } },
	{ BUTTON3_CLICKED,        { mouse_minimize,   { NULL  } } },
	{ BUTTON4_PRESSED,        { scrollback,       { "-1"  } } },
	{ BUTTON5_PRESSED,        { scrollback,       { "1"   } } },
};
#endif /* CONFIG_MOUSE */

static Cmd commands[] = {
	/* create [cmd]: create a new window, run `cmd` in the shell if specified */
	{ "create", { create,	{ NULL } } },
	/* focus <win_id>: focus the window whose `DVTM_WINDOW_ID` is `win_id` */
	{ "focus",  { focusid,	{ NULL } } },
	/* tag <win_id> <tag> [tag ...]: add +tag, remove -tag or set tag of the window with the given identifier */
	{ "tag",    { tagid,	{ NULL } } },
};

/* gets executed when dvtm is started */
static Action actions[] = {
	{ create, { NULL } },
};

static char const * const keytable[] = {
	/* add your custom key escape sequences */
};
