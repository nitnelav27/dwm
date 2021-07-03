/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  	= 7;        /* border pixel of windows */
static const unsigned int snap      	= 32;       /* snap pixel */
static const unsigned int gappih    	= 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    	= 10;       /* vert inner gap between windows */
static const unsigned int gappoh    	= 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    	= 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          	= 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            	= 1;        /* 0 means no bar */
static const int topbar             	= 1;        /* 0 means bottom bar */
static const char *fonts[]          	= { "Fira Sans:style=regular:size=13", "JoyPixels:style=Regular:size=13", "Font Awesome 5 Free Solid:style=Solid:size=13"};
static const char dmenufont[]       	= "Fira Sans:style=regular:size=13";
static const char col_black[]       	= "#000000";
static const char col_gray[]       	= "#222222";
static const char col_gray2[]       	= "#bbbbbb";
static const char col_white[]       	= "#ffffff";
static const char col_udec_blue[]       = "#003a66";
static const char col_udec_gold[]	= "#ffa102";
static const char col_red[]		= "#ac1d37";
static const char col_udec_silver[]	= "#8a8d8f";
static const char col_gmu_gren[]	= "#006633";
static const char col_gmu_gold[]	= "#ffcc33";
static const char col_gmu_turquoise[]	= "#00909e";
static const char col_brightblue[]	= "#3ec2cf";
static const char col_bluecomp[]	= "#cf4b3e";
static const char col_mycyan[]        	= "#005577";
static const char col_gray3[]		= "#282c34";
static const char *colors[][3]      	= {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_gray, col_black},
	[SchemeSel]  = { col_gray, col_brightblue,  col_gmu_turquoise},
	[SchemeStatus]  = {col_white, col_gray, col_black}, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = {col_gray, col_brightblue, col_black}, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { col_white, col_gray,  col_black}, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { col_white, col_gray,  col_black}, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { col_white, col_gray,  col_black}, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const char *const autostart[] = {
	"zsh", "-c", "feh --bg-scale --randomize $HOME/pix/wp", NULL, 
	"zsh", "-c", "picom -b", NULL,
	"zsh", "-c", "blueman-applet", NULL,
	"zsh", "-c", "dwmblocks", NULL,
	"zsh", "-c", "xautolock -time 15 -locker slock", NULL,
	"zsh", "-c", "dunst", NULL,
	"zsh", "-c", "trackpad", NULL,
	"zsh", "-c", "polkit-dumb-agent", NULL,
	NULL/*terminate*/ 
};

/* tagging */
static const char *tags[] = { "1: \U0001F4DA", "2: \U0001F4DA", "3: \U0001F4DA", "4: \U0001F4DA", "5: \U0001F4EC", "6: \U0001F4FA", "7: \U0001F3B5", "8: \u2699", "9: \U0001F4A9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      		instance    	title       	tags mask     	isfloating   monitor */
	{ "Gimp",     		NULL,       	NULL,       	0,            	1,           -1 },
	{ "Thunderbird",  	NULL,       	NULL,       	1 << 4,       	0,           -1 },
	{ "Spotify Premium",		NULL,	NULL, 	1 << 6,		0,           -1 },
	{ "ffplay",		NULL,		NULL,		0,		1,	     -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "MONO",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bluecomp, "-nf", col_gray, "-sb", col_brightblue, "-sf", col_gray, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *flap[] = {"flap", NULL};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        	function        argument */
	{ MODKEY,                       XK_p,      	spawn,          {.v = dmenucmd } },
	{ MODKEY,             		XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,			XK_w,	   	spawn,          SHCMD("google-chrome-stable")},
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD("emacs")},
	{ MODKEY,                       XK_b,      	togglebar,      {0} },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      	incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_m, 		zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      	incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      	incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      	incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      	incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      	incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      	incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      	incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      	incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      	incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      	incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      	incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      	incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      	incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      	incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      	togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      	defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      	killclient,     {0} },
	{ MODKEY,                       XK_t,      	setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      	setlayout,      {.v = &layouts[1]} },
	{ MODKEY,			XK_e,		spawn,		SHCMD("thunderbird")},
	{ MODKEY,                       XK_z,           spawn,          SHCMD("zathura")},
	{ MODKEY|ShiftMask,             XK_s,           spawn,          SHCMD("scrot -s ~/screenshots/$(date '+%y%m%d-%H%M').png")},
	/*{ MODKEY,                       XK_m,      	setlayout,      {.v = &layouts[2]} },*/
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	{ MODKEY,			XK_s,		togglesticky,	{0} },
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_p,		spawn,		{.v = flap}},
	{ MODKEY|ShiftMask,		XK_k,		spawn,		SHCMD("chlayout ; kill -46 $(pidof dwmblocks)")},
	{ MODKEY|ShiftMask,		XK_r,		spawn,		SHCMD("scasttoggle")},
	{ MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("camtoggle")},
	{ MODKEY, 			XK_r,		spawn,		SHCMD("st -e ranger ~")},
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD("spotify")},
	{ MODKEY|ShiftMask, 		XK_a,		spawn,		SHCMD("chsink")},
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)")},
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)")},
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("playerctl play-pause")},
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("playerctl next")},
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("playerctl previous")},


	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      	quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_e,      	quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

