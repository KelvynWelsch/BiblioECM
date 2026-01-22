# Sistema de Gerenciamento de Biblioteca

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows_(Native_Console)-lightgrey)
![Database](https://img.shields.io/badge/SQLite-3-green.svg)
![Status](https://img.shields.io/badge/Status-Production-brightgreen)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## Sobre o projeto

Este projeto consiste no desenvolvimento **pro-bono** de um software para atender às necessidades da biblioteca recém-inaugurada da **Primeira Igreja Batista em Taboão da Serra**, denominada **Biblioteca Edvaldo Cardoso Mattos**, uma biblioteca de pequeno porte que atualmente conta com aproximadamente **500 exemplares**.

O sistema foi desenvolvido com o objetivo de auxiliar nas operações básicas da biblioteca, como:

- Cadastro de usuários e livros  
- Controle de empréstimos, devoluções e renovações  
- Reservas de títulos  
- Verificação de bloqueios, atrasos e limites de empréstimos  

---

## Arquitetura e Tecnologias

O projeto utilizou as seguintes tecnologias:

- **Linguagem:** C++23.
- **Banco de Dados/Persistência:** SQLite3.

e foi estruturado seguindo os seguintes princípios de arquitetura:

- **Paradigma:** Orientação a Objetos com foco em **separação de responsabilidades (SoC)** 
- **Clean Architecture/Domain-Driven Design** simplificado
- **Arquietetura Hexagonal:** Banco de dados desacoplado via interfaces (port and adapter), permitindo a substituição da camada de dados com baixo impacto.
- **Comunicação entre Camadas:** Uso de DTOs (*Data Transfer Objects*) para o tráfego de informações entre o domínio e a interface.
- **Gerenciamento de Transações (ACID):** Implementação manual de controle transacional (`Begin`, `Commit`, `Rollback`) para garantir a integridade do banco de dados em caso de falha.


Estes princípios garantem que a lógica de negócio seja independente dos detalhes de infraestrutura e facilitam futuras modificações.

---

## Contexto e motivação

Este é o **primeiro projeto de software desse porte** desenvolvido pelo autor. Ao longo do desenvolvimento, o projeto passou por **diversas refatorações**, à medida que novos conceitos e boas práticas foram sendo aprendidos e aplicados.

Durante o processo, houve **grande auxílio de ferramentas de Inteligência Artificial**, especialmente para:

- Aceleração de escrita de *boilerplate*.
- Code Review e análise estática de segurança.
- Discussão de trade-offs arquiteturais.

*Nota: Todas as decisões finais de implementação e lógica de negócio foram validadas e auditadas pelo autor.*

---

## Aprendizados principais

O desenvolvimento deste sistema proporcionou aprendizados importantes, especialmente em relação a:

- Princípios de arquitetura de software  
- Uso e validação de entidades, *value objects* e identificadores tipados  
- Tratamento explícito de erros, evitando exceções e falhas silenciosas
- Recursos modernos de C++ como `std::expected` e `std::optional`

---

## Pontos de atenção para projetos futuros

Algumas decisões e dificuldades enfrentadas neste projeto servem como lições importantes para versões futuras ou novos projetos:

- **Tratamento de UTF-8 e encoding**  
  O tratamento de caracteres acentuados e encoding, inevitável em softwares pensados para a população lusófona, gerou grandes mudanças no meio do projeto e deveria ter sido definido com mais cuidado desde o início do projeto.

- **Convenções de nomenclatura**  
  Ao longo do desenvolvimento, houve mistura de idiomas (português e inglês).  
  Apesar de o inglês ser a língua franca da programação, o uso do português foi considerado necessário em alguns pontos, pois o público final do sistema é composto por usuários falantes de português.

- **Planejamento prévio de DTOs e erros**  
  O formato dos DTOs (*Data Transfer Objects*) e a estratégia de tratamento de erros poderiam ter sido melhor planejados desde o início, o que teria reduzido retrabalho e refatorações posteriores.

---

## Pontos fortes do projeto

Apesar de ser um projeto inicial, o sistema apresenta alguns pontos positivos relevantes:

- Tratamento de erros unificado e explícito, evitando exceções e facilitando o controle de fluxo (apesar de ainda haver espaço para melhorias)  
- Uso de interfaces para acesso ao banco de dados, permitindo desacoplamento da implementação concreta (SQLite) e facilitando futuras mudanças ou testes  
- Presença de um **ambiente de homologação**, permitindo a realização de testes e validações sem colocar em risco o banco de dados oficial da aplicação  

---

## Dívida Técnica e Roadmap de Melhorias


Como qualquer projeto em estágio inicial, este sistema possui limitações conhecidas. Os seguintes pontos foram identificados para refatoração em versões futuras:

### 1. Otimização de Recursos
- **Redução de Cópias:** Otimizar a passagem de objetos grandes para priorizar referências constantes (`const T&`) e o uso de `std::move`.
- **Uso de `std::string_view`:** Ampliar o uso de `string_view` em funções de busca e leitura para evitar alocações desnecessárias de memória em operações de string.

### 2. Consistência e Robustez
- **Transacionalidade:** Ampliar e simplificar o uso de transações em operações compostas.

Por fim, nota-se pequenas inconsistências de estilo devido à continua aprendizagem ao longo da construção do projeto.


## Evoluções Planejadas

### 1. Interface Gráfica
 A interface atual é baseada em terminal (CLI) e otimizada via WinAPI, priorizando baixo consumo de recursos. Para versões futuras, existe a intenção de:
  - Implementar uma **interface gráfica** mais amigável, utilizando uma biblioteca como **Qt** e consequentemente
  - Acabar com a atual dependência de plataformas específicas (Windows).
 
### 2. Notificações Automatizadas
Entre as principais funcionalidades que podem ser adicionadas, destaca-se:
  - Adicionar um **sistema de avisos por e-mail** (por exemplo, notificações de atraso ou disponibilidade de reservas), utilizando **SMTP**, possivelmente com o auxílio de bibliotecas como **libcurl**


---

## Considerações finais

Este projeto representa um **marco importante de aprendizado** para o autor, tanto do ponto de vista técnico quanto conceitual.

Ele cumpre seu propósito principal — atender uma biblioteca real — ao mesmo tempo em que serviu como um laboratório prático para o estudo de boas práticas de engenharia de software.

Melhorias e evoluções são esperadas naturalmente em versões futuras.

Sugestões e críticas são bem-vindas.

## Licença

Este projeto está licenciado sob a **Licença MIT** - veja o arquivo [LICENSE](LICENSE) para mais detalhes.

A escolha da licença permissiva MIT reflete o objetivo educacional e comunitário do projeto, permitindo que outros estudantes e desenvolvedores utilizem, estudem e modifiquem o código livremente.

## Autor

Desenvolvido por **Kelvyn Emmanoel de Castro Martins Welsch de Souza**
*Cientista de Dados e Pesquisador Quantitativo (PhD Candidate - USP)*

O projeto é Open Source e serve como estudo de caso para arquiteturas desacopladas em C++.
 