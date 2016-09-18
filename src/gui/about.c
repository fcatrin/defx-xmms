/* GTK About window */
#include <gtk/gtk.h>
#include <plugin.h>

/* window handle */
GtkWidget *about_win;

void about(void)
{
	GtkWidget *vbox, *frame, *label, *box, *button;

	if (about_win)return;
	about_win = gtk_window_new(GTK_WINDOW_DIALOG);
	gtk_signal_connect(GTK_OBJECT(about_win), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &about_win);
	gtk_window_set_title(GTK_WINDOW(about_win), "About DeFX");
	gtk_window_set_policy(GTK_WINDOW(about_win), FALSE, FALSE, FALSE);
	gtk_window_set_position(GTK_WINDOW(about_win), GTK_WIN_POS_MOUSE);
	gtk_container_border_width(GTK_CONTAINER(about_win), 5);

	vbox = gtk_vbox_new(FALSE, 2);
	gtk_container_add(GTK_CONTAINER(about_win), vbox);

	frame = gtk_frame_new(defxName);
	gtk_container_border_width(GTK_CONTAINER(frame), 5);
	gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

	label = gtk_label_new("\nMultiple effects processor for XMMS\n\n\n"
		"Author:  Franco Catrin L. <fcatrin@tuxpan.com>\n"
		"This plugin is released under the GPL\n"
		"http://defx.sourceforge.net\n");

	gtk_container_add(GTK_CONTAINER(frame), label);

	box = gtk_hbutton_box_new();
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(box), 5);
	gtk_box_pack_start(GTK_BOX(vbox), box, FALSE, FALSE, 0);

	button = gtk_button_new_with_label("Ok");
	gtk_signal_connect_object(GTK_OBJECT(button), "clicked", 
		GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(about_win));
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
	gtk_widget_grab_default(button);

	gtk_widget_show(button);
	gtk_widget_show(box);
	gtk_widget_show(frame);
	gtk_widget_show(label);
	gtk_widget_show(vbox);
	gtk_widget_show(about_win);
}
