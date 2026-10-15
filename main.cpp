#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Estrutura para guardar os dados da ROM detetada
struct RomInfo {
    std::string name;
    std::string systemType;
    size_t size;
    bool isValid;
};

// Função para detetar se é uma ROM de NES ou SNES com base no cabeçalho/extensão
RomInfo loadAndDetectRom(const std::string& filepath) {
    RomInfo info = {"Desconhecido", "Desconhecido", 0, false};
    
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return info;

    info.size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(info.size);
    file.read(buffer.data(), info.size);
    file.close();

    if (info.size < 16) return info;

    // Verificar cabeçalho NES (iNES: 'N', 'E', 'S', 0x1A)
    if (buffer[0] == 'N' && buffer[1] == 'E' && buffer[2] == 'E' && buffer[3] == 0x1A) { // Correção rápida de validação
        // Ajuste para NES
        if (buffer[0] == 'N' && buffer[1] == 'E' && buffer[2] == 'S' && buffer[3] == 0x1A) {
            info.systemType = "Nintendo Entertainment System (NES)";
            info.isValid = true;
        }
    } 
    // Se não for NES e tiver tamanho típico de SNES (múltiplos de 512 bytes de cabeçalho SMC ou direto SFC)
    else if (info.size > 32768) {
        info.systemType = "Super Nintendo (SNES)";
        info.isValid = true; // Formato standard de cartucho SNES
    }

    return info;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL2: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Criar janela dimensionada para o telemóvel na horizontal
    SDL_Window* window = SDL_CreateWindow("Emulador Multi-Sistema (NES & SNES)",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          960, 540,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    bool running = true;
    SDL_Event event;

    std::cout << "Emulador iniciado com sucesso na nuvem/nativo!" << std::endl;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Simulação de clique no botão "Carregar ROM" na interface gráfica
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                
                // Se clicar na zona do botão virtual de carregar ROM (exemplo de coordenadas)
                if (x >= 380 && x <= 580 && y >= 400 && y <= 450) {
                    std::cout << "Botao 'Carregar ROM' acionado! A procurar ficheiros..." << std::endl;
                    // Aqui entra o explorador de ficheiros nativo do Android / SDL
                }
            }
        }

        // Desenhar fundo do emulador (Azul clássico)
        SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
        SDL_RenderClear(renderer);

        // Desenhar um painel simulando o botão "Carregar ROM" no ecrã
        SDL_Rect buttonRect = { 380, 400, 200, 50 };
        SDL_SetRenderDrawColor(renderer, 255, 51, 51, 255); // Vermelho Mario
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
