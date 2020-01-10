/*
* gumTerm - A small and lightweight Terminal Application using GTK - Written by Adriano 'Gumball' M. (uwu pls call me Gumball)
* Licensed under The Unlicense.
* 
*/

#include <gtk/gtk.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <vte/vte.h>

#define CLR_R(x) (((x)&0xff0000) >> 16)
#define CLR_G(x) (((x)&0x00ff00) >> 8)
#define CLR_B(x) (((x)&0x0000ff) >> 0)
#define CLR_16(x) ((double)(x) / 0xff)
#define CLR_GDK(x, y) \
    (const GdkRGBA) { .red = CLR_16(CLR_R(x)), .green = CLR_16(CLR_G(x)), .blue = CLR_16(CLR_B(x)), .alpha = y }

int windowH = 30;
int windowW = 120;
double fontSize = 10.6;
bool allowBell = true;
bool allowBold = true;
bool allowHyper = true;
bool outScroll = true;
bool keyScroll = true;
int bgColor = 0x111111;
int fgColor = 0xffffff;
int curColor = 0xffffff;
// consoleFont = ???;
bool autoMouseHide = true;
long double cunt;  //Ah yes, this definetly needs to be here!

char *replace_char(char *str, char find, char replace) {
    char *current_pos = strchr(str, find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}

void parseConfigFile() {  // TODO Nothings done here yet.. hecc
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *delim = "=";

    fp = fopen(".gumdefaults", "r");  // Filename for testing...
    if (fp == NULL) {
        // TODO Create file with default config if it doesnt exist
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        // TODO
        // read is length and line is text
        if (read != 1) {
            char *ptr = strtok(line, delim);
            int init_size = strlen(line);
            while (ptr != NULL) {
                char *got = replace_char(replace_char(ptr, "\n", ""), "\r", "");
                char *currKey = got;
                ptr = strtok(NULL, delim);
                got = replace_char(replace_char(ptr, "\n", ""), "\r", "");
                char *currVal = got;
                ptr = strtok(NULL, delim);
                if (currKey = "KEYNAME") {     // ?
                    // TODO
                }
            }
        }
    }

    fclose(fp);
    if (line) {
        free(line);
    }
}

static gboolean on_title_changed(GtkWidget *terminal, gpointer user_data) {
    GtkWidget *window = user_data;
    gtk_window_set_title(window,
                         g_strconcat(vte_terminal_get_window_title(VTE_TERMINAL(terminal)) ?: "Terminal", " - gumTerm"));
    return TRUE;
}

int main(int argc, char *argv[]) {
    parseConfigFile();
    GtkWidget *window;
    GtkWidget *terminal, *box;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "gumTerm");

    vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), 10000);
    vte_terminal_set_scroll_on_output(VTE_TERMINAL(terminal), TRUE);
    vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(terminal), TRUE);
    vte_terminal_set_mouse_autohide(VTE_TERMINAL(terminal), TRUE);

    //vte_terminal_set_color_background(VTE_TERMINAL(terminal), (const GdkRGBA[]){CLR_GDK(0x000000, 1)});

    vte_terminal_set_colors(VTE_TERMINAL(terminal),
                            &CLR_GDK(0xffffff, 0),
                            //&(GdkRGBA){ .alpha = 0.85 },
                            (const GdkRGBA[]){CLR_GDK(0x111111, 1)},
                            (const GdkRGBA[]){
                                CLR_GDK(0x2E3436, 1),
                                CLR_GDK(0xCC0000, 1),
                                CLR_GDK(0x4E9A06, 1),
                                CLR_GDK(0xC4A000, 1),
                                CLR_GDK(0x3465A4, 1),
                                CLR_GDK(0x75507B, 1),
                                CLR_GDK(0x06989A, 1),
                                CLR_GDK(0xD3D7CF, 1),
                                CLR_GDK(0x555753, 1),
                                CLR_GDK(0xEF2929, 1),
                                CLR_GDK(0x8AE234, 1),
                                CLR_GDK(0xFCE94F, 1),
                                CLR_GDK(0x729FCF, 1),
                                CLR_GDK(0xAD7FA8, 1),
                                CLR_GDK(0x34E2E2, 1),
                                CLR_GDK(0xEEEEEC, 1)},
                            16);  // "Tango" Colorscheme

    g_signal_connect(terminal, "window-title-changed",
                     G_CALLBACK(on_title_changed), GTK_WINDOW(window));

    /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL};
    g_strfreev(envp);

    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
                             VTE_PTY_DEFAULT,
                             NULL,       /* working directory  */
                             command,    /* command */
                             NULL,       /* environment */
                             0,          /* spawn flags */
                             NULL, NULL, /* child setup */
                             NULL,       /* child pid */
                             -1,         /* timeout */
                             NULL, NULL, NULL);

    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    /* Create everything for the Menubar */
    GtkWidget *vbox;

    GtkWidget *menubar, *fileMenu, *fileMi, *quitMi, *segfaultMi;
    vbox = gtk_vbox_new(FALSE, 0);
    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();

    fileMi = gtk_menu_item_new_with_label("File");
    /* |-> */ segfaultMi = gtk_menu_item_new_with_label("C r a s h");
    /* |-> */ quitMi = gtk_menu_item_new_with_label("Quit");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), segfaultMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(segfaultMi), "activate",
                     G_CALLBACK(1), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
                     G_CALLBACK(gtk_main_quit), NULL);

    /* Put everything together and run the main loop */
    box = gtk_box_new(true, 0);
    gtk_box_pack_start(GTK_BOX(box), vbox, false, false, 0);

    //gtk_widget_set_opacity(GTK_WINDOW(window), 0.85);  //.85

    gtk_box_pack_end(GTK_BOX(box), terminal, true, true, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
    gtk_main();
}