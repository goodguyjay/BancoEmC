#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// a fim de não deixar o código muito extenso, decidi criar uma variável global para o número da conta
// favor não reproduzir essa prática em casa, variáveis globais são ruins :)
long int numeroDaConta_g = 0;

// usei a enumeração para definir os status de retorno das funções
// sucesso = 0, falha = 1, valor insuficiente = 2, cliente nulo = 3, valor invalido = 4
enum Status
{
	SUCESSO,
	FALHA,
	VALOR_INSUFICIENTE,
	CLIENTE_NULO,
	VALOR_INVALIDO
};

// struct cliente com os campos indicados pelo professor
typedef struct _cliente
{
	char nome[50];
	double saldo;
	long int numeroDaConta;
} Cliente;

// array de clientes com tamanho simbólico 10
Cliente* clientes[10];

// função para criar um cliente
// essa função recebe como parâmetro o nome do cliente e o saldo inicial
// aloca memória para um novo cliente, copia o nome e o saldo inicial para o novo cliente
// e retorna o novo cliente
Cliente* criarCliente(char* nomeCliente, double saldoInicial)
{
	Cliente* novoCliente = (Cliente*)calloc(1, sizeof(Cliente));
	strcpy_s(novoCliente->nome, sizeof(novoCliente->nome), nomeCliente);
	novoCliente->saldo = saldoInicial;

	// nesse trecho, incremento o número da conta a cada novo cliente criado e atribuo o valor ao novo cliente
	novoCliente->numeroDaConta = ++numeroDaConta_g;

	// nesse trecho, atribuo o novo cliente ao array de clientes
	clientes[numeroDaConta_g] = novoCliente;

	return novoCliente;
}

// função para sacar dinheiro
// essa função recebe como parâmetro um cliente e o valor a ser sacado
// verifica se o cliente é nulo, se o saldo é menor que o valor a ser sacado
// e retorna o status da operação com base no enum Status
int sacarDinheiro(Cliente* cliente, double valorASerSacado)
{
	if (cliente == NULL)
	{
		perror("Ponteiro cliente nulo.");
		return CLIENTE_NULO;
	}

	if (cliente->saldo < valorASerSacado)
	{
		puts("Saldo insuficiente. Tente novamente com um valor válido.");
		return VALOR_INSUFICIENTE;
	}

	cliente->saldo -= valorASerSacado;
	return SUCESSO;
}

// função para depositar dinheiro
// essa função recebe como parâmetro um cliente e o valor a ser depositado
// verifica se o cliente é nulo e se o valor a ser depositado é menor ou igual a zero
// e retorna o status da operação com base no enum Status
int depositarDinheiro(Cliente* cliente, double valorASerDepositado)
{
	if (cliente == NULL)
	{
		perror("Ponteiro cliente nulo.");
		return CLIENTE_NULO;
	}

	if (valorASerDepositado <= 0)
	{
		puts("Valor invalido. Tente novamente com um valor válido.");
		return VALOR_INVALIDO;
	}

	cliente->saldo += valorASerDepositado;
	return SUCESSO;
}

// função para transferir dinheiro
// essa função recebe como parâmetro o cliente de origem, o cliente de destino e o valor a ser transferido
// verifica se os clientes são nulos e se o saldo do cliente de origem é menor que o valor a ser transferido
// e retorna o status da operação com base no enum Status
int transferirDinheiro(Cliente* clienteOrigem, Cliente* clienteDestino, double valorATransferir)
{
	if (clienteOrigem == NULL || clienteDestino == NULL)
	{
		perror("Ponteiro 'cliente' nulo.");
		return CLIENTE_NULO;
	}

	if (clienteOrigem->saldo < valorATransferir)
	{
		puts("Saldo insuficiente. Tente novamente com um valor válido.");
		return VALOR_INSUFICIENTE;
	}

	clienteOrigem->saldo -= valorATransferir;
	clienteDestino->saldo += valorATransferir;
	return SUCESSO;
}

Cliente* buscarClientePorNumeroDaConta(long int numeroDaConta)
{
	// nesse trecho, percorro o array de clientes e verifico se o número da conta é igual ao número da conta passado como parâmetro
	// se for igual, retorno o cliente

	// esse for tá começando no 1 pq se colocar 0 o programa crasha e eu nao tenho ideia do pq
	for (int i = 1; i < sizeof(clientes); i++)
	{
		if (clientes[i]->numeroDaConta == numeroDaConta)
		{
			return clientes[i];
		}
	}

	return NULL;
}

void coletorDeLixo()
{
	// nesse trecho, libero automaticamente a memória alocada
	// é feita a iteração no array de clientes e a memória é liberada
	for (int i = 0; i < sizeof(clientes); i++)
	{
		free(clientes[i]);
	}
}

int main(int argc, char* argv[])
{
	// opção do menu inicializada com um valor padrão
	int opcao = 0;

	do {
		puts("\n----------------------------\n");
		puts("Bem vindo(a) ao Banco ABC!");
		puts("\n----------------------------\n");

		puts("Que operação você deseja fazer?\n");
		puts("[1]Abrir conta\n[2]Sacar dinheiro\n[3]Depositar dinheiro\n[4]Transferir dinheiro\n[0]Sair");
		scanf_s("%d", &opcao);

		switch (opcao)
		{
		case 0:
		{
			puts("Saindo do sistema...");
			break;
		}

		case 1:
		{
			char nomeCliente[50];
			double saldoInicial;

			puts("Digite o nome do cliente:");
			scanf_s("%s", nomeCliente, sizeof(nomeCliente));

			puts("Digite o saldo inicial:");
			scanf_s("%lf", &saldoInicial);

			Cliente* cliente = criarCliente(nomeCliente, saldoInicial);
			printf("Cliente %s criado com sucesso!\nSaldo inicial: R$%.2lf\nNúmero da conta: %ld", cliente->nome, cliente->saldo, cliente->numeroDaConta);
			break;
		}
		case 2:
		{
			double valorASerSacado;
			long int numeroDaContaDoCliente;

			puts("Digite o número da conta do cliente: ");

			// nesse trecho, verifico se o número da conta é válido
			// se o valor digitado for menor ou igual a zero ou maior que o número da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoCliente) <= 0 || numeroDaContaDoCliente > numeroDaConta_g)
			{
				puts("Número da conta inválido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser sacado:");

			scanf_s("%lf", &valorASerSacado);
			// nesse trecho, verifico se o valor a ser sacado é válido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorASerSacado <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor válido.");
				return VALOR_INVALIDO;
			}

			int status = sacarDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoCliente), valorASerSacado);
			if (status == SUCESSO)
			{
				printf("Saque de R$%.2lf realizado com sucesso!\nNome do cliente: %s\nNúmero da conta: %ld\nSaldo atual: R$%.2lf\n", valorASerSacado,
					buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->nome, numeroDaContaDoCliente, buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->saldo);
			}
			else
			{
				puts("Operação de saque falhou.");
				return FALHA;
			}
			break;
		}

		case 3:
		{
			double valorASerDepositado;
			long int numeroDaContaDoCliente;

			puts("Digite o número da conta do cliente: ");

			// nesse trecho, verifico se o número da conta é válido
			// se o valor digitado for menor ou igual a zero ou maior que o número da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoCliente) <= 0 || numeroDaContaDoCliente > numeroDaConta_g)
			{
				puts("Número da conta inválido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser depositado:");

			scanf_s("%lf", &valorASerDepositado);
			// nesse trecho, verifico se o valor a ser depositado é válido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorASerDepositado <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor válido.");
				return VALOR_INVALIDO;
			}

			int status = depositarDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoCliente), valorASerDepositado);
			if (status == SUCESSO)
			{
				printf("Depósito de R$%.2lf realizado com sucesso!\nNome do cliente: %s\nNúmero da conta: %ld\nSaldo atual: R$%.2lf\n", valorASerDepositado,
					buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->nome, numeroDaContaDoCliente, buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->saldo);
			}
			else
			{
				puts("Operação de depósito falhou.");
				return FALHA;
			}
			break;
		}

		case 4:
		{
			double valorATransferir;
			long int numeroDaContaDoClienteOrigem;
			long int numeroDaContaDoClienteDestino;

			puts("Digite o número da conta do cliente de origem: ");

			// nesse trecho, verifico se o número da conta é válido
			// se o valor digitado for menor ou igual a zero ou maior que o número da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoClienteOrigem) <= 0 || numeroDaContaDoClienteOrigem > numeroDaConta_g)
			{
				puts("Número da conta inválido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o número da conta do cliente de destino: ");

			// nesse trecho, verifico se o número da conta é válido
			// se o valor digitado for menor ou igual a zero ou maior que o número da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoClienteDestino) <= 0 || numeroDaContaDoClienteDestino > numeroDaConta_g)
			{
				puts("Número da conta inválido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser transferido:");

			scanf_s("%lf", &valorATransferir);
			// nesse trecho, verifico se o valor a ser transferido é válido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorATransferir <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor válido.");
				return VALOR_INVALIDO;
			}

			int status = transferirDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoClienteOrigem), buscarClientePorNumeroDaConta(numeroDaContaDoClienteDestino), valorATransferir);
			if (status == SUCESSO)
			{
				printf("Transferência de R$%.2lf realizada com sucesso!\nNome do cliente de origem: %s\nNúmero da conta de origem: %ld", valorATransferir,
					buscarClientePorNumeroDaConta(numeroDaContaDoClienteOrigem)->nome, numeroDaContaDoClienteOrigem);
			}
			else
			{
				puts("Operação de transferência falhou.");
				return FALHA;
			}
		}

		default:
		{
			puts("Opção inválida. Tente novamente.");
			break;
		}

		}
	} while (opcao != 0);

	coletorDeLixo();
	return 0;
}
