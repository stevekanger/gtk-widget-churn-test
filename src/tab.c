#include "tab.h"

G_DEFINE_TYPE(GwcTab, gwc_tab, GTK_TYPE_WIDGET)

// Just a global to show the rebuilding of based on active status
int active_tab = 0;

/**
 * The next 2 functions create a control.
 *
 * This creates a button in the left hand box that updates the tabs.
 * Meant to show that clicking this button does not increase memory in the same
 * manor.
 */
static void handle_control_click(
    GtkGestureClick *gesture,
    gint n_press,
    gdouble x,
    gdouble y,
    gpointer user_data
) {
    GtkBuilder *builder = user_data;

    if (active_tab >= 5) {
        active_tab = 1;
    } else {
        active_tab++;
    }

    GtkWidget *box_right =
        GTK_WIDGET(gtk_builder_get_object(builder, "box_right"));

    guint button =
        gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture));

    if (button == 1) {
        printf("Click control tab\n");
        update_tabs(box_right);
    }
}

GtkWidget *create_control(GtkBuilder *builder) {
    GtkWidget *widget = gtk_button_new_with_label("Control");

    GtkGesture *gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), 0);

    gtk_event_controller_set_propagation_phase(
        GTK_EVENT_CONTROLLER(gesture),
        GTK_PHASE_CAPTURE
    );

    gtk_widget_add_controller(
        GTK_WIDGET(widget),
        GTK_EVENT_CONTROLLER(gesture)
    );
    g_signal_connect(
        gesture,
        "pressed",
        G_CALLBACK(handle_control_click),
        builder
    );

    return widget;
}

/**
 * Tab update function.
 *
 * Removes all tabs from the right box and repopulates them.
 */
void update_tabs(GtkWidget *parent) {
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(GTK_WIDGET(parent))) != NULL) {
        gtk_box_remove(GTK_BOX(parent), child);
    }

    GtkWidget *tab1 = gwc_tab_new(1, active_tab == 1, "tab 1");
    GtkWidget *tab2 = gwc_tab_new(2, active_tab == 2, "tab 2");
    GtkWidget *tab3 = gwc_tab_new(3, active_tab == 3, "tab 3");
    GtkWidget *tab4 = gwc_tab_new(4, active_tab == 4, "tab 4");
    GtkWidget *tab5 = gwc_tab_new(5, active_tab == 5, "tab 5");

    gtk_box_append(GTK_BOX(parent), tab1);
    gtk_box_append(GTK_BOX(parent), tab2);
    gtk_box_append(GTK_BOX(parent), tab3);
    gtk_box_append(GTK_BOX(parent), tab4);
    gtk_box_append(GTK_BOX(parent), tab5);
}

static void handle_click(
    GtkGestureClick *gesture,
    gint n_press,
    gdouble x,
    gdouble y,
    gpointer user_data
) {
    GwcTab *tab = GWC_TAB(user_data);
    GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(tab));
    active_tab = tab->id;

    guint button =
        gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture));

    if (button == 1) {
        printf("Left click tab: %d\n", tab->id);
        update_tabs(parent);
    }

    if (button == 2) {
        printf("Middle click tab: %d\n", tab->id);
    }

    if (button == 3) {
        printf("Right click tab: %d\n", tab->id);
    }
}

static void gwc_tab_init(GwcTab *self) {
    // Create gesture for mouse buttons
    // 0 = listen to all buttons
    //
    // I shouldn't need to disconnect this per the docs it should be unrefed
    // when the widget is destoryed.
    // https://docs.gtk.org/gobject/signals.html#memory-management-of-signal-handlers

    GtkGesture *gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), 0);

    // gtk_event_controller_set_propagation_phase(
    //     GTK_EVENT_CONTROLLER(gesture),
    //     GTK_PHASE_CAPTURE
    // );

    gtk_widget_add_controller(GTK_WIDGET(self), GTK_EVENT_CONTROLLER(gesture));
    g_signal_connect(gesture, "pressed", G_CALLBACK(handle_click), self);
}

static void gwc_tab_dispose(GObject *obj) {
    GwcTab *self = GWC_TAB(obj);

    printf("calling dispose on %d\n", self->id);

    g_clear_pointer(&self->label, gtk_widget_unparent);

    G_OBJECT_CLASS(gwc_tab_parent_class)->dispose(obj);
}

static void gwc_tab_finalize(GObject *object) {
    GwcTab *self = GWC_TAB(object);

    g_free(self->name);

    printf("calling finalize on %d\n", self->id);

    // tried saving this to the struct and freeing just for good measure.
    //
    // gtk_widget_remove_controller(
    //     GTK_WIDGET(self),
    //     GTK_EVENT_CONTROLLER(self->gesture)
    // );

    G_OBJECT_CLASS(gwc_tab_parent_class)->finalize(object);
}

static void gwc_tab_class_init(GwcTabClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = gwc_tab_dispose;
    object_class->finalize = gwc_tab_finalize;

    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

GtkWidget *gwc_tab_new(int id, int focused, gchar *name) {
    GwcTab *self = g_object_new(GWC_TAB_TYPE, NULL);

    self->id = id;
    self->name = g_strdup(name);
    self->focused = focused;

    self->label = gtk_label_new(self->name);
    gtk_widget_set_parent(self->label, GTK_WIDGET(self));

    // Comment out this next line to stop memory increases
    gtk_widget_add_css_class(GTK_WIDGET(self), "tab");

    if (self->focused) {
        gtk_widget_add_css_class(GTK_WIDGET(self), "focused");
    }

    return GTK_WIDGET(self);
}
