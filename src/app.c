#include "app.h"
#include "tab.h"

G_DEFINE_TYPE(GwcApp, gwc_app, GTK_TYPE_APPLICATION)

static void load_css(GdkDisplay *display) {
    GtkCssProvider *css_provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(css_provider, "etc/style.css");
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(css_provider);
}

static void activate(GApplication *app) {
    GwcApp *gwc = GWC_APP(app);
    GdkDisplay *display = gdk_display_get_default();

    load_css(display);

    GtkWindow *window =
        GTK_WINDOW(gtk_builder_get_object(gwc->builder, "main_window"));

    // Associate window with the application
    gtk_window_set_application(window, GTK_APPLICATION(app));

    gtk_window_set_default_size(GTK_WINDOW(window), 600, 200);

    gtk_window_present(window);

    // Just add some elements. All will outlast the app except the first tab
    // which should print calling finalize on.
    GtkWidget *box_left =
        GTK_WIDGET(gtk_builder_get_object(gwc->builder, "box_left"));

    GtkWidget *control = create_control(gwc->builder);

    gtk_box_append(GTK_BOX(box_left), control);

    GtkWidget *box_right =
        GTK_WIDGET(gtk_builder_get_object(gwc->builder, "box_right"));

    GtkWidget *tab1 = gwc_tab_new(1, FALSE, "Starter Tab");

    gtk_box_append(GTK_BOX(box_right), tab1);
}

static void shutdown(GApplication *app) {
    G_APPLICATION_CLASS(gwc_app_parent_class)->shutdown(app);
}

static void dispose(GObject *obj) {
    GwcApp *gwc = GWC_APP(obj);

    if (gwc->builder) {
        g_object_unref(gwc->builder);
        gwc->builder = NULL;
    }

    G_OBJECT_CLASS(gwc_app_parent_class)->dispose(obj);
}

static void finalize(GObject *obj) {
    G_OBJECT_CLASS(gwc_app_parent_class)->finalize(obj);
}

static void startup(GApplication *app) {
    G_APPLICATION_CLASS(gwc_app_parent_class)->startup(app);

    GwcApp *gwc = GWC_APP(app);
    gwc->builder = gtk_builder_new_from_file("etc/layout.ui");
}

static void gwc_app_init(GwcApp *gwc) {
}

static void gwc_app_class_init(GwcAppClass *klass) {
    G_APPLICATION_CLASS(klass)->startup = startup;
    G_APPLICATION_CLASS(klass)->activate = activate;
    G_APPLICATION_CLASS(klass)->shutdown = shutdown;
    G_OBJECT_CLASS(klass)->dispose = dispose;
    G_OBJECT_CLASS(klass)->finalize = finalize;
}

GwcApp *gwc_app_new(void) {
    return g_object_new(
        GWC_APP_TYPE,
        "application-id",
        "com.exameple.Gwc",
        "flags",
        G_APPLICATION_DEFAULT_FLAGS,
        NULL
    );
}
