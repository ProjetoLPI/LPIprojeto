#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "funcoes_validacao.c"

//declarei a variável do caminho do arquivo
gchar *arquivo = NULL;

//função de escolher arquivo
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
        // Armazena o caminho na variável arquivo
        if (arquivo)
          g_free(arquivo);
        arquivo = g_strdup(caminho);

        // Imprime o diretório no console
        g_print("Arquivo selecionado: %s\n", arquivo);

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
//filtro de busca dos arquivos

//filtro 1: csv
  GtkFileFilter *filtro1 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro1, "CSV files");
  gtk_file_filter_add_pattern(filtro1, "*.csv");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro1);
//filtro 2: qualquer arquivo
  GtkFileFilter *filtro2 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro2, "Sem filtro");
  gtk_file_filter_add_pattern(filtro2, ".");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro2);

  // Conecta o sinal "response" do GtkFileChooserDialog ao callback
  g_signal_connect(file_dialog, "response", G_CALLBACK(on_file_chosen), NULL);

  // Exibe o seletor de arquivos
  gtk_window_present(GTK_WINDOW(file_dialog));
}
//função para verificar se arquivo tem o diretório
static void
on_validate_button_clicked(GtkButton *button, gpointer user_data)
{
  if (arquivo)
  {
    ValidarNulo(arquivo);
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
  GtkWidget *label;
  GtkWidget *show_file_button;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "ProjetoLPI");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_window_set_child(GTK_WINDOW(window), box);

  // Cria o botão para mostrar o seletor de arquivos
  button = gtk_button_new_with_label("Escolha o arquivo");
  gtk_box_append(GTK_BOX(box), button);
  


  gchar *transferir = g_object_get_data(G_OBJECT(button), "button_content");
  g_object_set_data(G_OBJECT(button2), "button_content", transferir);
  gtk_button_set_label(GTK_BUTTON(button2), transferir);

  button2 = gtk_button_new_with_label("Validar campos nulos");
  gtk_box_append(GTK_BOX(box), button2);

  g_signal_connect(button2, "clicked", G_CALLBACK(on_validate_button_clicked), NULL);



  // Conecta o sinal "clicked" do GtkButton para abrir o seletor de arquivos
  g_signal_connect(button, "clicked", G_CALLBACK(show_file_dialog), window);

  gtk_window_present(GTK_WINDOW(window));
}
//função main do gtk
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