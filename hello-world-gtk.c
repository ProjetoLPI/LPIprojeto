#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "funcoes_validacao.c"

// declarei a variável do caminho do arquivo
gchar *arquivo = NULL;

// função de escolher arquivo
static void on_file_chosen(GtkFileChooser *chooser, gint response_id) {
  if (response_id == GTK_RESPONSE_ACCEPT) {
    // Obtém o arquivo selecionado
    GFile *file = gtk_file_chooser_get_file(chooser);
    if (file) {
      // Obtém o caminho do arquivo
      gchar *caminho = g_file_get_path(file);
      if (caminho) {
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

static void show_file_dialog(GtkWindow *parent_window) {
  GtkWidget *file_dialog;

  // Cria o seletor de arquivos
  file_dialog = gtk_file_chooser_dialog_new(
      "Selecione o arquivo CSV", parent_window, GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancelar", GTK_RESPONSE_CANCEL,
      "_Abrir", GTK_RESPONSE_ACCEPT,
      NULL);

  gtk_window_set_transient_for(GTK_WINDOW(file_dialog), parent_window);

  // filtro de busca dos arquivos

  // filtro 1: csv
  GtkFileFilter *filtro1 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro1, "CSV files");
  gtk_file_filter_add_pattern(filtro1, "*.csv");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro1);
  // filtro 2: qualquer arquivo
  GtkFileFilter *filtro2 = gtk_file_filter_new();
  gtk_file_filter_set_name(filtro2, "Sem filtro");
  gtk_file_filter_add_pattern(filtro2, "*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro2);

  // Conecta o sinal "response" do GtkFileChooserDialog ao callback
  g_signal_connect(file_dialog, "response", G_CALLBACK(on_file_chosen), NULL);

  // Exibe o seletor de arquivos
  gtk_window_present(GTK_WINDOW(file_dialog));
}

// função para verificar se arquivo tem o diretório
static void on_validate_button_clicked(GtkButton *button, gpointer user_data) {
  if (arquivo) {
    ValidarNulo(arquivo);
  } else {
    g_print("Nenhum arquivo selecionado.\n");
  }
}

static void validarCEPbutton(GtkButton *button, gpointer user_data) {
  if (arquivo) {
    ValidarCEP(arquivo);
  } else {
    g_print("Nenhum arquivo selecionado.\n");
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *imagemUpe;
  GtkWidget *fixed;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "ProjetoLPI");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE); 
  gtk_window_set_child(GTK_WINDOW(window), grid);

 imagemUpe = gtk_image_new_from_file("/home/henriquessp/Desktop/LPI/projeto/imagemUpe.jpeg");
 gtk_grid_attach(GTK_GRID(grid), imagemUpe, 0, 0, 3, 1);

  // Cria o botão para mostrar o seletor de arquivos
  button = gtk_button_new_with_label("Escolha o arquivo");
  gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);
  
  // botão 2 (validar campos nulos)
  button2 = gtk_button_new_with_label("Validar campos nulos");
  gtk_grid_attach(GTK_GRID(grid), button2, 1, 1, 1, 1);
  g_signal_connect(button2, "clicked", G_CALLBACK(on_validate_button_clicked), NULL);
  gtk_widget_hide(button2);

  // botão 3 (validar cep)
  button3 = gtk_button_new_with_label("Validar CEP");
  gtk_grid_attach(GTK_GRID(grid), button3, 2, 1, 1, 1);
  g_signal_connect(button3, "clicked", G_CALLBACK(validarCEPbutton), NULL);
  gtk_widget_hide(button3);

  // Conecta o sinal "clicked" do GtkButton para abrir o seletor de arquivos
  g_signal_connect(button, "clicked", G_CALLBACK(show_file_dialog), window);

  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "style.css");
  GdkDisplay *display = gdk_display_get_default();
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  gtk_window_present(GTK_WINDOW(window));
}

// função main do gtk
int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}