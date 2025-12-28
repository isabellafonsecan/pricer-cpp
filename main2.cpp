#include <iostream>
#include <memory>
#include <string>
#include "options/vanillaEuropeanOption.hpp"
#include "options/asian_option.hpp"
#include "options/lookback_option.hpp"
#include "options/cliquet_option.hpp"
#include "options/option_a_barriere_desactivante.hpp"
#include "pricers/blackScholesMerton.hpp"
#include "pricers/monteCarlo.hpp"


// SDL2
#include <SDL2/SDL.h>

// OpenGL
#include <OpenGL/gl3.h>

// Dear ImGui
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

int main(int, char**)
{
    // -------------------------
    // SDL + OpenGL3 init
    // -------------------------
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Window* window = SDL_CreateWindow("Option Pricer GUI",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    // -------------------------
    // Variables
    // -------------------------
    double S0 = 100.0, K = 100.0, r = 0.05, sigma = 0.2, T = 1.0;
    int n_simulations = 100000;

    int optionChoice = 1;
    int pricerChoice = 1;
    double price = 0.0;
    bool priceComputed = false;

    bool isVanilla = true;

    // -------------------------
    // Main loop
    // -------------------------
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // -------------------------
        // Main window
        // -------------------------
        ImGui::Begin("Option Pricer");

        // Market parameters
        ImGui::InputDouble("Spot S0", &S0);
        ImGui::InputDouble("Strike K", &K);
        ImGui::InputDouble("Rate r", &r);
        ImGui::InputDouble("Volatility sigma", &sigma);
        ImGui::InputDouble("Maturity T", &T);
        ImGui::InputInt("Monte Carlo simulations", &n_simulations);

        // Option selection
        const char* options[] = {
            "Vanilla Call", "Vanilla Put", "Asian Call", "Asian Put",
            "Lookback Call", "Lookback Put", "Cliquet Call", "Cliquet Put",
            "Barrier Call", "Barrier Put"
        };
        ImGui::Combo("Option Type", &optionChoice, options, IM_ARRAYSIZE(options));
        isVanilla = (optionChoice == 0 || optionChoice == 1);

        // Pricer selection
        if (isVanilla) {
            const char* pricers[] = { "Black-Scholes-Merton", "Monte Carlo" };
            ImGui::Combo("Pricer", &pricerChoice, pricers, IM_ARRAYSIZE(pricers));
        } else {
            ImGui::Text("Pricer: Monte Carlo (only model for exotics)");
            pricerChoice = 2;
        }

        // Price button
        if (ImGui::Button("Compute Price"))
        {
            std::unique_ptr<Option> option;
            switch(optionChoice)
            {
                case 0: option = std::make_unique<VanillaEuropeanOption>(K,T,true); break;
                case 1: option = std::make_unique<VanillaEuropeanOption>(K,T,false); break;
                case 2: option = std::make_unique<AsianOption>(K,T,true); break;
                case 3: option = std::make_unique<AsianOption>(K,T,false); break;
                case 4: option = std::make_unique<LookbackOption>(K,T,true); break;
                case 5: option = std::make_unique<LookbackOption>(K,T,false); break;
                case 6: option = std::make_unique<CliquetOption>(K,T,true); break;
                case 7: option = std::make_unique<CliquetOption>(K,T,false); break;
                case 8: option = std::make_unique<barriereOption>(K,T,true,80.0,barriereOption::DOWN_AND_OUT); break;
                case 9: option = std::make_unique<barriereOption>(K,T,false,120.0,barriereOption::UP_AND_OUT); break;
            }

            std::unique_ptr<Pricer> pricer;
            if(isVanilla) {
                if(pricerChoice == 1)
                    pricer = std::make_unique<BlackScholesMerton>(S0,K,r,sigma);
                else
                    pricer = std::make_unique<MonteCarloPricer>(S0,r,sigma,n_simulations);
            } else {
                pricer = std::make_unique<MonteCarloPricer>(S0,r,sigma,n_simulations);
            }

            price = pricer->price(*option);
            priceComputed = true;
        }

        if(priceComputed) {
            ImGui::Text("Option price: %.4f", price);
        }

        ImGui::End();

        // -------------------------
        // Rendering
        // -------------------------
        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // -------------------------
    // Cleanup
    // -------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
