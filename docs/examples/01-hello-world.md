# Tutorial 01: Hello World

Seu primeiro diálogo com Showbox.

**Nível:** 🟢 Básico  
**Tempo estimado:** 5 minutos

---

## Objetivo

Criar um diálogo simples com uma mensagem e um botão para fechar.

---

## Código Completo

```bash
#!/bin/bash
#
# Tutorial 01: Hello World
# O exemplo mais simples de Showbox

showbox << 'EOF'
# Configurar título da janela
set title "Meu Primeiro Diálogo"

# Adicionar um rótulo com mensagem de boas-vindas
add label "<h2>Olá, Mundo!</h2><p>Bem-vindo ao Showbox!</p>" lbl_welcome

# Adicionar um botão para fechar
add pushbutton "OK" btn_ok exit default

# Mostrar a janela
show
EOF

echo "Diálogo fechado!"
```

---

## Passo a Passo

### 1. O Heredoc

```bash
showbox << 'EOF'
...
EOF
```

O heredoc (`<< 'EOF'`) permite enviar múltiplas linhas para o showbox. As aspas em `'EOF'` impedem a expansão de variáveis dentro do bloco.

### 2. Configurar a Janela

```bash
set title "Meu Primeiro Diálogo"
```

O comando `set` sem nome de widget configura a janela principal.

### 3. Adicionar Widgets

```bash
add label "<h2>Olá, Mundo!</h2>" lbl_welcome
```

- `add` - Comando para adicionar widget
- `label` - Tipo do widget (rótulo de texto)
- `"<h2>..."` - Conteúdo (suporta HTML básico)
- `lbl_welcome` - Nome identificador (opcional)

### 4. Botão de Ação

```bash
add pushbutton "OK" btn_ok exit default
```

- `pushbutton` - Tipo botão clicável
- `"OK"` - Texto do botão
- `btn_ok` - Nome identificador
- `exit` - Fecha o diálogo ao clicar
- `default` - Botão padrão (ativado por Enter)

### 5. Mostrar

```bash
show
```

Exibe a interface construída.

---

## Executando

```bash
chmod +x hello_world.sh
./hello_world.sh
```

---

## Resultado

Uma janela aparecerá com a mensagem "Olá, Mundo!" e um botão OK. Ao clicar no botão ou pressionar Enter, a janela fecha e o script continua.

---

## Experimente!

Modifique o código para:

1. Trocar a mensagem por seu nome
2. Adicionar uma segunda linha de texto
3. Mudar o texto do botão para "Fechar"

---

## Próximo Tutorial

[02. Formulário de Login →](02-formulario-login.md)
