#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "funcoes_validacao.c"

gchar *global_caminho = NULL;

static void
on_file_chosen(GtkFileChooser *chooser,
               gint            response_id)
{
  if (response_id == GTK_RESPONSE_ACCEPT)
  {
    // Obtém o arquivo selecionado
    GFile *file = gtk_file_chooser_get_file(chooser);
    if (file)
    {
      // Obtém o caminho do arquivo
      gchar *caminho = g_file_get_path(file);
      if (caminho)
      {
        // Armazena o caminho na variável global
        if (global_caminho)
          g_free(global_caminho);
        global_caminho = g_strdup(caminho);

        // Imprime o diretório no console
        g_print("Arquivo selecionado: %s\n", global_caminho);

        g_free(caminho);
      }

      g_object_unref(file);
    }
  }

  // Fecha o seletor de arquivos
  gtk_window_destroy(GTK_WINDOW(chooser));
}

static void
show_file_dialog(GtkWindow *parent_window)
{
  GtkWidget *file_dialog;

  // Cria o seletor de arquivos
  file_dialog = gtk_file_chooser_dialog_new(
      "Select Files",
      parent_window,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Open", GTK_RESPONSE_ACCEPT,
      NULL);

  gtk_window_set_transient_for(GTK_WINDOW(file_dialog), parent_window);

  GtkFileFilter *filtro1 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro1, "CSV files");
  gtk_file_filter_add_pattern(filtro1, "*.csv");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro1);

  GtkFileFilter *filtro2 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro2, "Sem filtro");
  gtk_file_filter_add_pattern(filtro2, ".");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro2);

  // Conecta o sinal "response" do GtkFileChooserDialog ao callback
  g_signal_connect(file_dialog, "response", G_CALLBACK(on_file_chosen), NULL);

  // Exibe o seletor de arquivos
  gtk_window_present(GTK_WINDOW(file_dialog));
}

static void
on_validate_button_clicked(GtkButton *button, gpointer user_data)
{
  if (global_caminho)
  {
    ValidarNulo(global_caminho);
  }
  else
  {
    g_print("Nenhum arquivo selecionado.\n");
  }
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;
  GtkWidget *button2;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "ProjetoLPI");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_window_set_child(GTK_WINDOW(window), box);

  // Cria o botão para mostrar o seletor de arquivos
  button = gtk_button_new_with_label("Escolha o arquivo");
  gtk_box_append(GTK_BOX(box), button);

  // Conecta o sinal "clicked" do GtkButton para abrir o seletor de arquivos
  g_signal_connect(button, "clicked", G_CALLBACK(show_file_dialog), window);

  button2 = gtk_button_new_with_label("Validar campos nulos");
  gtk_box_append(GTK_BOX(box), button2);

  // Conecta o sinal "clicked" do GtkButton para validar campos nulos
  g_signal_connect(button2, "clicked", G_CALLBACK(on_validate_button_clicked), NULL);

  gtk_window_present(GTK_WINDOW(window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
