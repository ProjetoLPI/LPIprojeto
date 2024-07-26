#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "funcoes_validacao.c"

// Variável global para armazenar o caminho do arquivo
gchar *arquivo = NULL;

// Ponteiros para os botões e widgets de texto
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4; 
GtkWidget *button5; 
GtkWidget *button6;

// Função callback chamada ao escolher o arquivo
static void on_file_chosen(GtkFileChooser *chooser, gint response_id) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        GFile *file = gtk_file_chooser_get_file(chooser);
        if (file) {
            gchar *caminho = g_file_get_path(file);
            if (caminho) {
                if (arquivo)
                    g_free(arquivo);
                arquivo = g_strdup(caminho);

                // Mostrar os botões 2, 3, 4 e 5 apenas se o arquivo selecionado for um CSV
                const char *file_extension = strrchr(caminho, '.');
                if (file_extension && strcmp(file_extension, ".csv") == 0) {
                    gtk_widget_set_visible(button2, TRUE);
                    gtk_widget_set_visible(button3, TRUE);
                    gtk_widget_set_visible(button4, TRUE);
                    gtk_widget_set_visible(button5, TRUE);
                    gtk_widget_set_visible(button6, TRUE);

                } else {
                    gtk_widget_set_visible(button2, FALSE);
                    gtk_widget_set_visible(button3, FALSE);
                    gtk_widget_set_visible(button4, FALSE);
                    gtk_widget_set_visible(button5, FALSE);
                    gtk_widget_set_visible(button6, TRUE);
                }

                g_free(caminho);
            }
            g_object_unref(file);
        }
    }
    gtk_window_destroy(GTK_WINDOW(chooser));
}

// Função para mostrar o diálogo de seleção de arquivos
static void show_file_dialog(GtkWindow *parent_window) {
    GtkWidget *file_dialog = gtk_file_chooser_dialog_new(
        "Selecionar Arquivo CSV", GTK_WINDOW(parent_window), GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancelar", GTK_RESPONSE_CANCEL,
        "_Abrir", GTK_RESPONSE_ACCEPT,
        NULL);

    gtk_window_set_transient_for(GTK_WINDOW(file_dialog), parent_window);

    GtkFileFilter *filtro_csv = gtk_file_filter_new();
    gtk_file_filter_set_name(filtro_csv, "Arquivos CSV");
    gtk_file_filter_add_pattern(filtro_csv, "*.csv");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro_csv);

    GtkFileFilter *filtro_todos = gtk_file_filter_new();
    gtk_file_filter_set_name(filtro_todos, "Todos os arquivos");
    gtk_file_filter_add_pattern(filtro_todos, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog), filtro_todos);

    g_signal_connect(file_dialog, "response", G_CALLBACK(on_file_chosen), NULL);

    gtk_window_present(GTK_WINDOW(file_dialog));
}

// Função callback chamada ao clicar no botão de validação de nulos
static void on_validate_button_clicked(GtkButton *button, gpointer user_data) {
    if (arquivo) {
        ValidarNulo(arquivo);
        g_print("Validação de campos nulos realizada.");
    } else {
        g_print("Nenhum arquivo selecionado.");
    }
}

// Função callback chamada ao clicar no botão de validação de CEP
static void validarCEPbutton(GtkButton *button, gpointer user_data) {
    if (arquivo) {
        ValidarCEP(arquivo);
        g_print("Validação de CEP realizada.");
    } else {
        g_print("Nenhum arquivo selecionado.");
    }
}

// Função callback chamada ao clicar no botão validar prontuario 
static void validarProntuariobutton(GtkButton *button, gpointer user_data) {
    if (arquivo) {
        ValidarProntuario(arquivo);
        g_print("Validação de prontuário realizada.");
    } else {
        g_print("Nenhum arquivo selecionado.");
    }
}

// Função callback chamada ao clicar no botão validar CNS
static void validarCNSbutton(GtkButton *button, gpointer user_data) {
    if (arquivo) {
        ValidarCNS(arquivo);
        g_print("Validação de CNS realizada.");
    } else {
        g_print("Nenhum arquivo selecionado.");
    }
}

// Função callback chamada ao clicar no botão validar idade
static void validarIdadebutton(GtkButton *button, gpointer user_data) {
    if (arquivo) {
        ValidarIdade(arquivo);
        g_print("Validação de idade realizada.");
    } else {
        g_print("Nenhum arquivo selecionado.");
    }
}

// Função chamada ao ativar o aplicativo
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *imagemCisam;
    GtkWidget *imagemUpe;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Validador de formulários do CISAM");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Torna as linhas homogêneas
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Adiciona um espaço vazio na primeira linha e coluna para centralizar
    GtkWidget *empty_space1 = gtk_label_new(NULL);
    gtk_grid_attach(GTK_GRID(grid), empty_space1, 0, 0, 1, 1);

    // Carregar e mostrar a imagem no topo da janela
    imagemCisam = gtk_image_new_from_file("./PNG-MARCA-NUTES-CISAM-VERTICAL-1024x1024.png");
    gtk_grid_attach(GTK_GRID(grid), imagemCisam, 1, 0, 1, 1);

    // Criação dos botões
    button = gtk_button_new_with_label("Escolher Arquivo CSV");
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

    button2 = gtk_button_new_with_label("Validar Campos Nulos");
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 2, 1, 1);
    gtk_widget_set_visible(button2, FALSE); // Esconde o botão inicialmente

    button3 = gtk_button_new_with_label("Validar CEP");
    gtk_grid_attach(GTK_GRID(grid), button3, 1, 3, 1, 1);
    gtk_widget_set_visible(button3, FALSE); // Esconde o botão inicialmente

    button4 = gtk_button_new_with_label("Validar Prontuário");
    gtk_grid_attach(GTK_GRID(grid), button4, 1, 4, 1, 1);
    gtk_widget_set_visible(button4, FALSE);

    button5 = gtk_button_new_with_label("Validar CNS");
    gtk_grid_attach(GTK_GRID(grid), button5, 1, 5, 1, 1);
    gtk_widget_set_visible(button5, FALSE);

    button6 = gtk_button_new_with_label("Validar Idade");
    gtk_grid_attach(GTK_GRID(grid), button6, 1, 6, 1, 1);
    gtk_widget_set_visible(button6, FALSE);



    // Define que os botões devem se expandir e preencher as células
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    gtk_widget_set_hexpand(button2, TRUE);
    gtk_widget_set_vexpand(button2, TRUE);
    gtk_widget_set_hexpand(button3, TRUE);
    gtk_widget_set_vexpand(button3, TRUE);
    gtk_widget_set_hexpand(button4, TRUE);
    gtk_widget_set_vexpand(button4, TRUE);
    gtk_widget_set_hexpand(button5, TRUE);
    gtk_widget_set_vexpand(button5, TRUE);
    gtk_widget_set_hexpand(button6, TRUE);
    gtk_widget_set_vexpand(button6, TRUE);

    // Define o tamanho mínimo dos botões
    gtk_widget_set_size_request(button, 100, 50);
    gtk_widget_set_size_request(button2, 100, 50);
    gtk_widget_set_size_request(button3, 100, 50);
    gtk_widget_set_size_request(button4, 100, 50);
    gtk_widget_set_size_request(button5, 100, 50);
    gtk_widget_set_size_request(button6, 100, 50);

    // Adiciona um espaço vazio na última linha e coluna para centralizar
    GtkWidget *empty_space2 = gtk_label_new(NULL);
    gtk_grid_attach(GTK_GRID(grid), empty_space2, 2, 4, 1, 1);

    // Conexão dos sinais aos callbacks
    g_signal_connect(button, "clicked", G_CALLBACK(show_file_dialog), window);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_validate_button_clicked), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(validarCEPbutton), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(validarProntuariobutton), NULL);
    g_signal_connect(button5, "clicked", G_CALLBACK(validarCNSbutton), NULL);
    g_signal_connect(button6, "clicked", G_CALLBACK(validarIdadebutton), NULL);

    // Carregar e aplicar o CSS para estilização
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./style.css");
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_window_present(GTK_WINDOW(window));
}

// Função main
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}