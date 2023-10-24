#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a user
typedef struct User {
    char name[50];
    struct User* next;
} User;

// Structure to represent an edge (connection)
typedef struct Edge {
    User* user;
    struct Edge* next;
} Edge;

// Structure to represent a social network graph
typedef struct Graph {
    User* users;
    Edge** adjacency_list;
    int num_users;
} Graph;

Graph* graph;

// Function to create a new user
User* createUser(const char* name) {
    User* user = (User*)malloc(sizeof(User));
    strcpy(user->name, name);
    user->next = NULL;
    return user;
}

// Function to create a new edge (connection)
Edge* createEdge(User* user) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->user = user;
    edge->next = NULL;
    return edge;
}

// Function to initialize a social network graph
Graph* createGraph(int num_users) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->users = NULL;
    graph->num_users = num_users;
    graph->adjacency_list = (Edge**)malloc(num_users * sizeof(Edge*));
    for (int i = 0; i < num_users; i++) {
        graph->adjacency_list[i] = NULL; // Initialize to NULL
    }
    return graph;
}
// Function to search for a user by name and return a pointer to the user
User* searchUserInGraph(Graph* graph, const char* username) {
    User* user = graph->users;
    while (user) {
        if (strcmp(user->name, username) == 0) {
            return user;
        }
        user = user->next;
    }
    return NULL;
}


// Function to add a user to the graph
void addUser(Graph* graph, const char* name) {
    User* newUser = createUser(name);
    newUser->next = graph->users;
    graph->users = newUser;
    printf("User '%s' added.\n", name);
}

// Function to add a connection between two users
void addConnection(Graph* graph, const char* user1, const char* user2) {
    User* u1 = NULL;
    User* u2 = NULL;

    // Find user1 and user2 in the list of users
    User* current = graph->users;
    while (current) {
        if (strcmp(current->name, user1) == 0) {
            u1 = current;
        } else if (strcmp(current->name, user2) == 0) {
            u2 = current;
        }
        current = current->next;
    }

    if (u1 == NULL || u2 == NULL) {
        printf("User not found!\n");
        return;
    }

    // Create edges to represent the connection
    Edge* edge1 = createEdge(u2);
    edge1->next = graph->adjacency_list[u1 - graph->users];
    graph->adjacency_list[u1 - graph->users] = edge1;

    Edge* edge2 = createEdge(u1);
    edge2->next = graph->adjacency_list[u2 - graph->users];
    graph->adjacency_list[u2 - graph->users] = edge2;
     printf("Connection between '%s' and '%s' made.\n", user1, user2);
}

// Function to display all users and their connections
// Function to display all users and their connections
void displayGraph(Graph* graph, GtkTextBuffer *buffer) {
    User* user = graph->users;
    while (user) {
        gchar *text = g_strdup_printf("User: %s, Connections:", user->name);
        gtk_text_buffer_insert_at_cursor(buffer, text, -1);
        g_free(text);

        Edge* edge = graph->adjacency_list[user - graph->users];
        while (edge) {
            gchar *edge_text = g_strdup_printf(" %s", edge->user->name);
            gtk_text_buffer_insert_at_cursor(buffer, edge_text, -1);
            g_free(edge_text);
            edge = edge->next;
        }
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);

        user = user->next;
    }
}


// Callback function for adding a user
void addUserCallback(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(data));
    if (strlen(username) > 0) {
        addUser(graph, username);
        // Update the user interface if needed
    }
}

// Callback function for creating a connection
// Callback function for creating a connection
void addConnectionCallback(GtkWidget *widget, gpointer data) {
    const char *connection = gtk_entry_get_text(GTK_ENTRY(data));
    char user1[50], user2[50];

    // Parse the input and check for valid input
    if (sscanf(connection, "%s %s", user1, user2) == 2) {
        // Check if both users exist in the graph
        User* u1 = searchUserInGraph(graph, user1);
        User* u2 = searchUserInGraph(graph, user2);

        if (u1 != NULL && u2 != NULL) {
            addConnection(graph, user1, user2);
            // Update the user interface if needed
        } else {
            printf("User not found!\n");
        }
    } else {
        printf("Invalid input. Use 'User1 User2' format.\n");
    }
}


// Callback function for displaying the network
// Callback function for displaying the network
// Callback function for displaying the network
void displayGraphCallback(GtkWidget *widget, gpointer data) {
    // Create a new window to display the network
    GtkWidget *window;
    GtkWidget *text_view;
    GtkWidget *scrolled_window;
    GtkTextBuffer *buffer;

    // Initialize GTK
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Social Network");


    text_view = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Clear the existing text in the buffer
    gtk_text_buffer_set_text(buffer, "", -1);

    // Display the network in the text view
    displayGraph(graph, buffer);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();
}


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *addUserButton;
    GtkWidget *addConnectionButton;
    GtkWidget *displayNetworkButton;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize the graph
    graph = createGraph(100);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Social Network");

    // Create a vertical box to hold widgets
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create labels and entries for adding users and connections
    label1 = gtk_label_new("Enter username to add:");
    entry1 = gtk_entry_new();
    label2 = gtk_label_new("Enter User1 User2 for connection:");
    entry2 = gtk_entry_new();
    addUserButton = gtk_button_new_with_label("Add User");
    addConnectionButton = gtk_button_new_with_label("Create Connection");

    // Connect the callbacks to the buttons
    g_signal_connect(G_OBJECT(addUserButton), "clicked", G_CALLBACK(addUserCallback), entry1);
    g_signal_connect(G_OBJECT(addConnectionButton), "clicked", G_CALLBACK(addConnectionCallback), entry2);

    // Create a button to display the network
    displayNetworkButton = gtk_button_new_with_label("Display Network");
    g_signal_connect(G_OBJECT(displayNetworkButton), "clicked", G_CALLBACK(displayGraphCallback), NULL);

    // Add widgets to the box
    gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entry1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), addUserButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entry2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), addConnectionButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), displayNetworkButton, FALSE, FALSE, 0);

    // Display the window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    // Clean up memory (you should free all allocated memory before exiting)
    free(graph->adjacency_list);
    free(graph);

    return 0;
}