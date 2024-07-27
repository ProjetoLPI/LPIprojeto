# Projeto de Validação de Dados de Pacientes do CISAM com Interface Gráfica em C e GTK

## Introdução

Este projeto visa validar planilhas de Excel contendo dados de pacientes do CISAM, integrando funções de validação a uma interface gráfica interativa utilizando o framework GTK na linguagem C.

## Objetivo

Assegurar a precisão e integridade dos dados de pacientes através de um sistema automatizado de validação, proporcionando uma interface gráfica amigável para os usuários.

## Funcionalidades

### Validação de Dados

1. **Datas e Horários**: Verificação do formato e limites aceitáveis.
2. **Códigos Postais (CEPs)**: Verificação de CEPs com exatamente 8 dígitos.
3. **Campos Nulos em CSV**: Identificação de colunas vazias.
4. **Prontuário**: Verificação se contém apenas números.
5. **Idade**: Verificação se a idade contém apenas números e está entre 0 e 120.

### Interface Gráfica

1. **Seleção de Arquivos**: Permite ao usuário selecionar arquivos CSV para validação.
2. **Validação e Feedback**: Processa os arquivos e fornece feedback sobre erros encontrados.

## Interface Gráfica com GTK

1. **Botão de Seleção de Arquivo**: Permite ao usuário escolher o arquivo CSV.
2. **Botão de Validação**: Inicia o processo de validação dos dados.
3. **Feedback Visual**: Mostra o status da validação e os erros encontrados.

## Dificuldades

1. **Padrões de CSV Inconsistentes**: 
   - Necessidade de criar funções para localizar os índices das colunas.
   - Validação adaptativa para lidar com colunas contadas ou não.

## Benefícios do Projeto

1. **Precisão e Confiabilidade**: Minimiza erros em processos manuais.
2. **Eficiência**: Automatiza a validação, economizando tempo.
3. **Facilidade de Uso**: Interface gráfica intuitiva.
4. **Redução de Erros Manuais**: Aumenta a confiabilidade das informações.
5. **Melhoria na Qualidade dos Dados**: Garante que todos os campos obrigatórios estejam preenchidos corretamente.
6. **Adaptação e Expansibilidade**: Sistema facilmente adaptável e expansível.

## Estrutura do Projeto

- `validadorCsv.c`: Interface gráfica e integração com o backend.
- `funcoes_validacao.c`: Funções de validação.

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests.

## Licença

Este projeto está licenciado sob a Licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---
