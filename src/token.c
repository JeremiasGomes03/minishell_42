/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/17 19:35:02 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
//////////////////////////////////////////////////////
// Função init_tokenizer():
//     Alocar memória para estrutura de tokenizador
//     Copiar a string de entrada para o tokenizador
//     Inicializar variáveis de controle:
//         - Posição (posição atual na string de entrada)
//         - Estado de aspas (fora ou dentro de aspas)
//         - Lista de tokens (vazia no início)

/////////////////////////////////////////////////////
// Função tokenization_loop():
//     Enquanto não atingir o fim da string de entrada:
//         Se estamos dentro de aspas (in_quotes):
//             Chamar handle_quoted_content
//         Se o caractere for espaço:
//             Chamar handle_space
//         Se o caractere for um operador (ex: '>', '<', '|'):
//             Chamar handle_operator
//         Se for um caractere comum (parte de uma palavra):
//             Chamar handle_regular_char
//         Avançar para o próximo caractere na string

/////////////////////////////////////////////////////////
// Função handle_quoted_content():
//     Se o caractere atual for uma aspa:
//         Se for o fechamento de uma aspa correspondente (verificar com matching_quote):
//             Chamar end_quote_state
//         Caso contrário, iniciar aspas (chamar start_quote_state)
//     Caso contrário, adicionar caractere ao token atual (chamar add_to_current_token)

/////////////////////////////////////////////////////////
// Função handle_space():
//     Se o token atual não estiver vazio:
//         Adicionar o token à lista de tokens
//         Iniciar um novo token vazio
//     Avançar para o próximo caractere

// Função handle_operator():
//     Se o operador for duplo (ex: '>>'):
//         Criar um token com tipo OPERADOR_DUPO
//     Caso contrário, criar um token com tipo OPERADOR
//     Adicionar token à lista de tokens
//     Avançar para o próximo caractere
	
// Função handle_regular_char():
//     Coletar caracteres até encontrar um separador (como espaço ou operador)
//     Criar um token do tipo PALAVRA (WORD)
//     Adicionar caracteres ao token enquanto a sequência de caracteres for válida
//     Se atingir um separador (espaço ou operador):
//         Adicionar o token à lista de tokens
//         Iniciar um novo token vazio

/////////////////////////////////////////////////////
// Função handle_regular_char():
//     Coletar caracteres até encontrar um separador (como espaço ou operador)
//     Criar um token do tipo PALAVRA (WORD)
//     Adicionar caracteres ao token enquanto a sequência de caracteres for válida
//     Se atingir um separador (espaço ou operador):
//         Adicionar o token à lista de tokens
//         Iniciar um novo token vazio

