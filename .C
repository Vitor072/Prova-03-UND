#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura Produto
struct Produto {
    char nome[50];
    float preco;
    char categoria[20];
};

// Nó da Lista de Produtos
struct ProdutoNode {
    struct Produto produto;
    struct ProdutoNode* next;
};

// Estrutura Usuário
struct Usuario {
    char nome[50];
    char senha[50];
    char email[50];
};

// Nó da Lista de Usuários
struct UsuarioNode {
    struct Usuario usuario;
    struct UsuarioNode* next;
};

// Estrutura Carrinho
struct CarrinhoNode {
    struct Produto produto;
    struct CarrinhoNode* next;
};

// Função de login
int login(struct UsuarioNode* headUsuarios) {
    char nome[50];
    char senha[50];
    char email[50];
    int tentativas = 3;

    while (tentativas > 0) {
        printf("Login de Usuário:\n");
        scanf("%s", nome);
        printf("Senha:\n");
        scanf("%s", senha);
        printf("Email:\n");
        scanf("%s", email);

        struct UsuarioNode* current = headUsuarios;
        while (current != NULL) {
            if (strcmp(current->usuario.nome, nome) == 0 && 
                strcmp(current->usuario.senha, senha) == 0 && 
                strcmp(current->usuario.email, email) == 0) {
                printf("Login Efetuado Com Sucesso!\n");
                return 1;
            }
            current = current->next;
        }

        tentativas--;
        printf("Usuário, Senha ou Email INCORRETO!\nTente Novamente!\n\nTentativas Restantes: %d\n\n\n", tentativas);
    }

    return 0;
}

// Função de boas-vindas
void mensagemBoasVindas() {
    printf("------------------------------------\n\n\n");
    printf("Bem-vindo à AMERICAN IMPORTS\n\n\n");
    printf("------------------------------------\n");
}

// Função para cadastrar produto
void cadastrarProduto(struct ProdutoNode** headProdutos) {
    struct ProdutoNode* novo = (struct ProdutoNode*)malloc(sizeof(struct ProdutoNode));
    if (novo == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    printf("Informe o nome, categoria e o preço do produto:\n");
    scanf("%49s %19s %f", novo->produto.nome, novo->produto.categoria, &novo->produto.preco);
    novo->next = NULL;

    // Inserir no final da lista
    if (*headProdutos == NULL) {
        *headProdutos = novo;
    } else {
        struct ProdutoNode* temp = *headProdutos;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo;
    }

    printf("Produto cadastrado com sucesso!\n");
}

// Função para listar produtos
void listarProdutos(struct ProdutoNode* headProdutos) {
    printf("--- Lista de Produtos ---\n");
    int i = 1;
    struct ProdutoNode* current = headProdutos;
    while (current != NULL) {
        printf("%d. %s (%s) - R$ %.2f\n", i, current->produto.nome, current->produto.categoria, current->produto.preco);
        current = current->next;
        i++;
    }
    printf("=========================\n");
}

// Função para buscar produto por nome (busca linear para listas encadeadas)
struct ProdutoNode* buscarProduto(struct ProdutoNode* head, const char* nome) {
    struct ProdutoNode* current = head;
    while (current != NULL) {
        if (strcmp(current->produto.nome, nome) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Função para editar produto
void editarProduto(struct ProdutoNode* headProdutos) {
    char nome[50];
    printf("Digite o nome do produto a ser editado: ");
    scanf("%49s", nome);

    struct ProdutoNode* produto = buscarProduto(headProdutos, nome);
    if (produto != NULL) {
        printf("Informe o novo nome, categoria e o novo preço do produto:\n");
        scanf("%49s %19s %f", produto->produto.nome, produto->produto.categoria, &produto->produto.preco);
        printf("Produto editado com sucesso!\n");
    } else {
        printf("Produto não encontrado!\n");
    }
}

// Função para excluir produto
void excluirProduto(struct ProdutoNode** headProdutos) {
    char nome[50];
    printf("Digite o nome do produto a ser excluído: ");
    scanf("%49s", nome);

    struct ProdutoNode* current = *headProdutos;
    struct ProdutoNode* anterior = NULL;

    while (current != NULL && strcmp(current->produto.nome, nome) != 0) {
        anterior = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Produto não encontrado!\n");
        return;
    }

    if (anterior == NULL) {
        // Remover o primeiro nó
        *headProdutos = current->next;
    } else {
        anterior->next = current->next;
    }

    free(current);
    printf("Produto excluído com sucesso!\n");
}

// Função para salvar os produtos em um arquivo binário
void salvarProdutos(struct ProdutoNode* headProdutos) {
    FILE* arquivo = fopen("produtos.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Contar a quantidade de produtos
    int qtdProdutos = 0;
    struct ProdutoNode* current = headProdutos;
    while (current != NULL) {
        qtdProdutos++;
        current = current->next;
    }

    fwrite(&qtdProdutos, sizeof(int), 1, arquivo);

    // Escrever os produtos
    current = headProdutos;
    while (current != NULL) {
        fwrite(&(current->produto), sizeof(struct Produto), 1, arquivo);
        current = current->next;
    }

    fclose(arquivo);
    printf("Produtos salvos com sucesso!\n");
}

// Função para carregar os produtos de um arquivo binário
void carregarProdutos(struct ProdutoNode** headProdutos) {
    FILE* arquivo = fopen("produtos.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    int qtdProdutos;
    fread(&qtdProdutos, sizeof(int), 1, arquivo);

    // Limpar a lista atual
    while (*headProdutos != NULL) {
        struct ProdutoNode* temp = *headProdutos;
        *headProdutos = (*headProdutos)->next;
        free(temp);
    }

    for (int i = 0; i < qtdProdutos; i++) {
        struct ProdutoNode* novo = (struct ProdutoNode*)malloc(sizeof(struct ProdutoNode));
        if (novo == NULL) {
            printf("Erro de alocação de memória.\n");
            fclose(arquivo);
            return;
        }
        fread(&(novo->produto), sizeof(struct Produto), 1, arquivo);
        novo->next = NULL;

        // Inserir no final da lista
        if (*headProdutos == NULL) {
            *headProdutos = novo;
        } else {
            struct ProdutoNode* temp = *headProdutos;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = novo;
        }
    }

    fclose(arquivo);
    printf("Produtos carregados com sucesso!\n");
}

// Função para adicionar usuário (para inicialização)
void adicionarUsuario(struct UsuarioNode** headUsuarios, const char* nome, const char* senha, const char* email) {
    struct UsuarioNode* novo = (struct UsuarioNode*)malloc(sizeof(struct UsuarioNode));
    if (novo == NULL) {
        printf("Erro de alocação de memória para usuário.\n");
        return;
    }
    strcpy(novo->usuario.nome, nome);
    strcpy(novo->usuario.senha, senha);
    strcpy(novo->usuario.email, email);
    novo->next = NULL;

    // Inserir no final da lista
    if (*headUsuarios == NULL) {
        *headUsuarios = novo;
    } else {
        struct UsuarioNode* temp = *headUsuarios;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo;
    }
}

// Função para exibir catálogo de produtos por categoria
void catalogoProdutos(struct ProdutoNode* headProdutos) {
    printf("--- Catálogo de Produtos ---\n");

    const char* categorias[] = {"Camisas", "Calças", "Shorts", "Tênis"};
    int numCategorias = sizeof(categorias) / sizeof(categorias[0]);

    for (int c = 0; c < numCategorias; c++) {
        printf("Categoria: %s\n", categorias[c]);
        int count = 1;
        struct ProdutoNode* current = headProdutos;
        while (current != NULL) {
            if (strcmp(current->produto.categoria, categorias[c]) == 0) {
                printf("  %d. %s - R$ %.2f\n", count, current->produto.nome, current->produto.preco);
                count++;
            }
            current = current->next;
        }
        printf("\n");
    }
    printf("=========================\n");
}

// Função para adicionar produto ao carrinho
void comprarProduto(struct ProdutoNode* headProdutos, struct CarrinhoNode** headCarrinho) {
    listarProdutos(headProdutos);
    int escolha;
    printf("Digite o número do produto que deseja adicionar ao carrinho: ");
    scanf("%d", &escolha);

    // Encontrar o produto escolhido
    int i = 1;
    struct ProdutoNode* current = headProdutos;
    while (current != NULL && i < escolha) {
        current = current->next;
        i++;
    }

    if (current == NULL) {
        printf("Número do produto inválido!\n");
        return;
    }

    // Adicionar ao carrinho
    struct CarrinhoNode* novo = (struct CarrinhoNode*)malloc(sizeof(struct CarrinhoNode));
    if (novo == NULL) {
        printf("Erro de alocação de memória para o carrinho.\n");
        return;
    }
    novo->produto = current->produto;
    novo->next = NULL;

    if (*headCarrinho == NULL) {
        *headCarrinho = novo;
    } else {
        struct CarrinhoNode* temp = *headCarrinho;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo;
    }

    printf("Produto '%s' adicionado ao carrinho.\n", novo->produto.nome);
}

// Função para exibir os produtos no carrinho
void mostrarCarrinho(struct CarrinhoNode* headCarrinho) {
    printf("--- Carrinho de Compras ---\n");

    if (headCarrinho == NULL) {
        printf("O carrinho está vazio.\n");
    } else {
        int i = 1;
        struct CarrinhoNode* current = headCarrinho;
        while (current != NULL) {
            printf("%d. %s - R$ %.2f\n", i, current->produto.nome, current->produto.preco);
            current = current->next;
            i++;
        }
    }
    printf("=========================\n");
}

// Função para remover produto do carrinho
void removerDoCarrinho(struct CarrinhoNode** headCarrinho) {
    mostrarCarrinho(*headCarrinho);
    int escolha;
    printf("Digite o número do produto que deseja remover do carrinho: ");
    scanf("%d", &escolha);

    struct CarrinhoNode* current = *headCarrinho;
    struct CarrinhoNode* anterior = NULL;
    int i = 1;

    while (current != NULL && i < escolha) {
        anterior = current;
        current = current->next;
        i++;
    }

    if (current == NULL) {
        printf("Número do produto inválido!\n");
        return;
    }

    if (anterior == NULL) {
        // Remover o primeiro nó
        *headCarrinho = current->next;
    } else {
        anterior->next = current->next;
    }

    free(current);
    printf("Produto removido do carrinho.\n");
}

// Função para calcular o total do carrinho
float calcularTotalCarrinho(struct CarrinhoNode* headCarrinho) {
    float total = 0.0;
    struct CarrinhoNode* current = headCarrinho;
    while (current != NULL) {
        total += current->produto.preco;
        current = current->next;
    }
    return total;
}

// Função para finalizar compra
void finalizarCompra(struct CarrinhoNode** headCarrinho) {
    if (*headCarrinho == NULL) {
        printf("O carrinho está vazio! Não é possível finalizar a compra.\n");
        return;
    }

    float total = calcularTotalCarrinho(*headCarrinho);
    printf("=== Finalizar Compra ===\n");
    printf("O total da sua compra é: R$ %.2f\n", total);
    printf("Compra realizada com sucesso!\n");

    // Esvaziar o carrinho após finalizar a compra
    while (*headCarrinho != NULL) {
        struct CarrinhoNode* temp = *headCarrinho;
        *headCarrinho = (*headCarrinho)->next;
        free(temp);
    }
    printf("O carrinho foi esvaziado.\n");
}

// Função para exibir o menu
void menu() {
    printf("=== Escolha uma opção ===\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Ver Carrinho\n");
    printf("3 - Editar produto\n");
    printf("4 - Remover do carrinho\n");
    printf("5 - Comprar produto\n");
    printf("6 - Ver catálogo de produtos\n");
    printf("7 - Finalizar compra\n");
    printf("8 - Salvar produtos\n");
    printf("9 - Carregar produtos\n");
    printf("10 - Sair\n");
    printf("=========================\n");
}

// Função para liberar a lista de produtos
void liberarProdutos(struct ProdutoNode* headProdutos) {
    while (headProdutos != NULL) {
        struct ProdutoNode* temp = headProdutos;
        headProdutos = headProdutos->next;
        free(temp);
    }
}

// Função para liberar a lista de usuários
void liberarUsuarios(struct UsuarioNode* headUsuarios) {
    while (headUsuarios != NULL) {
        struct UsuarioNode* temp = headUsuarios;
        headUsuarios = headUsuarios->next;
        free(temp);
    }
}

// Função para liberar o carrinho
void liberarCarrinho(struct CarrinhoNode* headCarrinho) {
    while (headCarrinho != NULL) {
        struct CarrinhoNode* temp = headCarrinho;
        headCarrinho = headCarrinho->next;
        free(temp);
    }
}

// Função principal
int main() {
    // Inicializar listas
    struct ProdutoNode* headProdutos = NULL;
    struct UsuarioNode* headUsuarios = NULL;
    struct CarrinhoNode* headCarrinho = NULL;

    // Adicionar produtos pré-cadastrados
    struct Produto produtosIniciais[] = {
        {"Camisa Polo Azul Claro", 79.90, "Camisas"},
        {"Camisa Social Branca Slim Fit", 99.00, "Camisas"},
        {"Camisa de Linho Bege", 120.00, "Camisas"},
        {"Camisa Xadrez Vermelha", 89.90, "Camisas"},
        {"Camisa Jeans Manga Longa", 110.00, "Camisas"},
        {"Calça Jeans Skinny Azul", 129.90, "Calças"},
        {"Calça Chino Caqui", 139.00, "Calças"},
        {"Calça Moletom Cinza Mescla", 99.00, "Calças"},
        {"Calça Social Preta Slim Fit", 149.90, "Calças"},
        {"Calça Cargo Verde Militar", 119.00, "Calças"},
        {"Bermuda Jeans Rasgada", 89.90, "Shorts"},
        {"Short Moletom Preto", 59.90, "Shorts"},
        {"Bermuda Sarja Bege", 79.00, "Shorts"},
        {"Short Esportivo Cinza", 49.90, "Shorts"},
        {"Bermuda Estampada Floral", 69.90, "Shorts"},
        {"Tênis Branco Casual", 199.90, "Tênis"},
        {"Tênis Esportivo Preto", 249.90, "Tênis"},
        {"Tênis Slip On Azul Marinho", 179.00, "Tênis"},
        {"Tênis de Corrida Cinza", 299.90, "Tênis"},
        {"Tênis Chunky Branco", 269.90, "Tênis"}
    };
    int numProdutosIniciais = sizeof(produtosIniciais) / sizeof(produtosIniciais[0]);
    for (int i = 0; i < numProdutosIniciais; i++) {
        struct ProdutoNode* novo = (struct ProdutoNode*)malloc(sizeof(struct ProdutoNode));
        if (novo == NULL) {
            printf("Erro de alocação de memória para produtos iniciais.\n");
            liberarProdutos(headProdutos);
            return 1;
        }
        novo->produto = produtosIniciais[i];
        novo->next = headProdutos;
        headProdutos = novo;
    }

    // Adicionar usuários pré-cadastrados
    adicionarUsuario(&headUsuarios, "vitor", "vitor123", "vitor@example.com");
    adicionarUsuario(&headUsuarios, "daniel", "daniel321", "daniel@example.com");
    adicionarUsuario(&headUsuarios, "vdaniel", "vitord2", "vdaniel@example.com");
    adicionarUsuario(&headUsuarios, "daniel3", "vitor3", "daniel3@example.com");
    adicionarUsuario(&headUsuarios, "vitor55", "daniel1234", "vitor55@example.com");

    // Login do usuário
    if (!login(headUsuarios)) {
        printf("Número de tentativas excedido. Programa encerrado.\n");
        liberarProdutos(headProdutos);
        liberarUsuarios(headUsuarios);
        return 1;
    }

    mensagemBoasVindas();

    int op;
    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &op);
        switch (op) {
            case 1:
                cadastrarProduto(&headProdutos);
                break;
            case 2:
                mostrarCarrinho(headCarrinho);
                break;
            case 3:
                editarProduto(headProdutos);
                break;
            case 4:
                removerDoCarrinho(&headCarrinho);
                break;
            case 5:
                comprarProduto(headProdutos, &headCarrinho);
                break;
            case 6:
                catalogoProdutos(headProdutos);
                break;
            case 7:
                finalizarCompra(&headCarrinho);
                break;
            case 8:
                salvarProdutos(headProdutos);
                break;
            case 9:
                carregarProdutos(&headProdutos);
                break;
            case 10:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (op != 10);

    // Liberar memória antes de encerrar
    liberarProdutos(headProdutos);
    liberarUsuarios(headUsuarios);
    liberarCarrinho(headCarrinho);

    return 0;
}