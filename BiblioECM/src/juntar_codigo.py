import os

# Extensões que queremos ler
EXTENSOES = ('.h', '.hpp', '.cpp', '.c')
# Pastas para ignorar (opcional)
IGNORAR = {'build', '.git', '.vscode', 'bin', 'obj'}

def juntar_arquivos():
    with open('PROJETO_COMPLETO.txt', 'w', encoding='utf-8') as saida:
        for root, dirs, files in os.walk('.'):
            # Remove pastas ignoradas da busca
            dirs[:] = [d for d in dirs if d not in IGNORAR]
            
            for file in files:
                if file.endswith(EXTENSOES):
                    caminho_completo = os.path.join(root, file)
                    saida.write(f"\n{'='*20}\n")
                    saida.write(f"// ARQUIVO: {caminho_completo}\n")
                    saida.write(f"{'='*20}\n")
                    
                    try:
                        with open(caminho_completo, 'r', encoding='latin-1') as f:
                            saida.write(f.read())
                            saida.write("\n")
                    except Exception as e:
                        saida.write(f"// Erro ao ler arquivo: {e}\n")

    print("Concluído! O código foi salvo em 'PROJETO_COMPLETO.txt'.")

if __name__ == '__main__':
    juntar_arquivos()
