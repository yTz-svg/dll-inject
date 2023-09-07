# Injetor de DLL em Processos

Este é um exemplo de código em C que demonstra como injetar uma DLL em um processo Windows. Este código permite a injeção segura de uma DLL em um processo alvo usando funções da API do Windows.

## Uso

1. Compile o código-fonte fornecido usando um compilador C compatível com o Windows.

2. Substitua as seguintes variáveis no código pelo seu valor apropriado:
   - `dwProcessId`: O ID do processo alvo no qual você deseja injetar a DLL.
   - `lpszDLLPath`: O caminho completo para a DLL que você deseja injetar.

3. Execute o programa compilado. Ele tentará injetar a DLL no processo especificado.

4. Verifique o resultado da injeção no console.

## Requisitos

- Um ambiente de desenvolvimento C compatível com o Windows.
- Um processo alvo para a injeção da DLL.

## Atenção

A injeção de DLL em processos é uma operação sensível e deve ser realizada de forma ética e legal. Certifique-se de ter permissão para realizar essa operação no processo alvo e cumpra todas as leis e regulamentos aplicáveis.

## Licença

Este código é fornecido sob a Licença MIT. Consulte o arquivo [LICENSE](LICENSE) para obter detalhes.

---

**Nota:** Este código é fornecido apenas como um exemplo educacional e de demonstração. A utilização de técnicas de injeção de DLL em processos deve ser feita com responsabilidade e em conformidade com todas as leis e regulamentos aplicáveis.
