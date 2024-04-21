#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// a fim de n�o deixar o c�digo muito extenso, decidi criar uma vari�vel global para o n�mero da conta
// favor n�o reproduzir essa pr�tica em casa, vari�veis globais s�o ruins :)
long int numeroDaConta_g = 0;

// usei a enumera��o para definir os status de retorno das fun��es
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

// array de clientes com tamanho simb�lico 10
Cliente* clientes[10];

// fun��o para criar um cliente
// essa fun��o recebe como par�metro o nome do cliente e o saldo inicial
// aloca mem�ria para um novo cliente, copia o nome e o saldo inicial para o novo cliente
// e retorna o novo cliente
Cliente* criarCliente(char* nomeCliente, double saldoInicial)
{
	Cliente* novoCliente = (Cliente*)calloc(1, sizeof(Cliente));
	strcpy_s(novoCliente->nome, sizeof(novoCliente->nome), nomeCliente);
	novoCliente->saldo = saldoInicial;

	// nesse trecho, incremento o n�mero da conta a cada novo cliente criado e atribuo o valor ao novo cliente
	novoCliente->numeroDaConta = ++numeroDaConta_g;

	// nesse trecho, atribuo o novo cliente ao array de clientes
	clientes[numeroDaConta_g] = novoCliente;

	return novoCliente;
}

// fun��o para sacar dinheiro
// essa fun��o recebe como par�metro um cliente e o valor a ser sacado
// verifica se o cliente � nulo, se o saldo � menor que o valor a ser sacado
// e retorna o status da opera��o com base no enum Status
int sacarDinheiro(Cliente* cliente, double valorASerSacado)
{
	if (cliente == NULL)
	{
		perror("Ponteiro cliente nulo.");
		return CLIENTE_NULO;
	}

	if (cliente->saldo < valorASerSacado)
	{
		puts("Saldo insuficiente. Tente novamente com um valor v�lido.");
		return VALOR_INSUFICIENTE;
	}

	cliente->saldo -= valorASerSacado;
	return SUCESSO;
}

// fun��o para depositar dinheiro
// essa fun��o recebe como par�metro um cliente e o valor a ser depositado
// verifica se o cliente � nulo e se o valor a ser depositado � menor ou igual a zero
// e retorna o status da opera��o com base no enum Status
int depositarDinheiro(Cliente* cliente, double valorASerDepositado)
{
	if (cliente == NULL)
	{
		perror("Ponteiro cliente nulo.");
		return CLIENTE_NULO;
	}

	if (valorASerDepositado <= 0)
	{
		puts("Valor invalido. Tente novamente com um valor v�lido.");
		return VALOR_INVALIDO;
	}

	cliente->saldo += valorASerDepositado;
	return SUCESSO;
}

// fun��o para transferir dinheiro
// essa fun��o recebe como par�metro o cliente de origem, o cliente de destino e o valor a ser transferido
// verifica se os clientes s�o nulos e se o saldo do cliente de origem � menor que o valor a ser transferido
// e retorna o status da opera��o com base no enum Status
int transferirDinheiro(Cliente* clienteOrigem, Cliente* clienteDestino, double valorATransferir)
{
	if (clienteOrigem == NULL || clienteDestino == NULL)
	{
		perror("Ponteiro 'cliente' nulo.");
		return CLIENTE_NULO;
	}

	if (clienteOrigem->saldo < valorATransferir)
	{
		puts("Saldo insuficiente. Tente novamente com um valor v�lido.");
		return VALOR_INSUFICIENTE;
	}

	clienteOrigem->saldo -= valorATransferir;
	clienteDestino->saldo += valorATransferir;
	return SUCESSO;
}

Cliente* buscarClientePorNumeroDaConta(long int numeroDaConta)
{
	// nesse trecho, percorro o array de clientes e verifico se o n�mero da conta � igual ao n�mero da conta passado como par�metro
	// se for igual, retorno o cliente

	// esse for t� come�ando no 1 pq se colocar 0 o programa crasha e eu nao tenho ideia do pq
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
	// nesse trecho, libero automaticamente a mem�ria alocada
	// � feita a itera��o no array de clientes e a mem�ria � liberada
	for (int i = 0; i < sizeof(clientes); i++)
	{
		free(clientes[i]);
	}
}

int main(int argc, char* argv[])
{
	// op��o do menu inicializada com um valor padr�o
	int opcao = 0;

	do {
		puts("\n----------------------------\n");
		puts("Bem vindo(a) ao Banco ABC!");
		puts("\n----------------------------\n");

		puts("Que opera��o voc� deseja fazer?\n");
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
			printf("Cliente %s criado com sucesso!\nSaldo inicial: R$%.2lf\nN�mero da conta: %ld", cliente->nome, cliente->saldo, cliente->numeroDaConta);
			break;
		}
		case 2:
		{
			double valorASerSacado;
			long int numeroDaContaDoCliente;

			puts("Digite o n�mero da conta do cliente: ");

			// nesse trecho, verifico se o n�mero da conta � v�lido
			// se o valor digitado for menor ou igual a zero ou maior que o n�mero da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoCliente) <= 0 || numeroDaContaDoCliente > numeroDaConta_g)
			{
				puts("N�mero da conta inv�lido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser sacado:");

			scanf_s("%lf", &valorASerSacado);
			// nesse trecho, verifico se o valor a ser sacado � v�lido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorASerSacado <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor v�lido.");
				return VALOR_INVALIDO;
			}

			int status = sacarDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoCliente), valorASerSacado);
			if (status == SUCESSO)
			{
				printf("Saque de R$%.2lf realizado com sucesso!\nNome do cliente: %s\nN�mero da conta: %ld\nSaldo atual: R$%.2lf\n", valorASerSacado,
					buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->nome, numeroDaContaDoCliente, buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->saldo);
			}
			else
			{
				puts("Opera��o de saque falhou.");
				return FALHA;
			}
			break;
		}

		case 3:
		{
			double valorASerDepositado;
			long int numeroDaContaDoCliente;

			puts("Digite o n�mero da conta do cliente: ");

			// nesse trecho, verifico se o n�mero da conta � v�lido
			// se o valor digitado for menor ou igual a zero ou maior que o n�mero da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoCliente) <= 0 || numeroDaContaDoCliente > numeroDaConta_g)
			{
				puts("N�mero da conta inv�lido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser depositado:");

			scanf_s("%lf", &valorASerDepositado);
			// nesse trecho, verifico se o valor a ser depositado � v�lido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorASerDepositado <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor v�lido.");
				return VALOR_INVALIDO;
			}

			int status = depositarDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoCliente), valorASerDepositado);
			if (status == SUCESSO)
			{
				printf("Dep�sito de R$%.2lf realizado com sucesso!\nNome do cliente: %s\nN�mero da conta: %ld\nSaldo atual: R$%.2lf\n", valorASerDepositado,
					buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->nome, numeroDaContaDoCliente, buscarClientePorNumeroDaConta(numeroDaContaDoCliente)->saldo);
			}
			else
			{
				puts("Opera��o de dep�sito falhou.");
				return FALHA;
			}
			break;
		}

		case 4:
		{
			double valorATransferir;
			long int numeroDaContaDoClienteOrigem;
			long int numeroDaContaDoClienteDestino;

			puts("Digite o n�mero da conta do cliente de origem: ");

			// nesse trecho, verifico se o n�mero da conta � v�lido
			// se o valor digitado for menor ou igual a zero ou maior que o n�mero da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoClienteOrigem) <= 0 || numeroDaContaDoClienteOrigem > numeroDaConta_g)
			{
				puts("N�mero da conta inv�lido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o n�mero da conta do cliente de destino: ");

			// nesse trecho, verifico se o n�mero da conta � v�lido
			// se o valor digitado for menor ou igual a zero ou maior que o n�mero da conta global
			// exibo uma mensagem de erro e retorno FALHA
			if (scanf_s("%ld", &numeroDaContaDoClienteDestino) <= 0 || numeroDaContaDoClienteDestino > numeroDaConta_g)
			{
				puts("N�mero da conta inv�lido. Tente novamente.");
				return FALHA;
			}

			puts("Digite o valor a ser transferido:");

			scanf_s("%lf", &valorATransferir);
			// nesse trecho, verifico se o valor a ser transferido � v�lido
			// se o valor digitado for menor ou igual a zero, exibo uma mensagem de erro e retorno VALOR_INVALIDO
			if (valorATransferir <= 0)
			{
				puts("Valor invalido. Tente novamente com um valor v�lido.");
				return VALOR_INVALIDO;
			}

			int status = transferirDinheiro(buscarClientePorNumeroDaConta(numeroDaContaDoClienteOrigem), buscarClientePorNumeroDaConta(numeroDaContaDoClienteDestino), valorATransferir);
			if (status == SUCESSO)
			{
				printf("Transfer�ncia de R$%.2lf realizada com sucesso!\nNome do cliente de origem: %s\nN�mero da conta de origem: %ld", valorATransferir,
					buscarClientePorNumeroDaConta(numeroDaContaDoClienteOrigem)->nome, numeroDaContaDoClienteOrigem);
			}
			else
			{
				puts("Opera��o de transfer�ncia falhou.");
				return FALHA;
			}
		}

		default:
		{
			puts("Op��o inv�lida. Tente novamente.");
			break;
		}

		}
	} while (opcao != 0);

	coletorDeLixo();
	return 0;
}
