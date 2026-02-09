#include <unordered_map>
#include <BBOP/Graphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


/**
 * @brief Gère les ressource 
 *
 * @details Permet de charger les textures et sons au lancement du jeu et évite 
 * l'utilisation du disque dur pendant que le jeu tourne
 */
class RessourceManager {
  private:
    static std::unordered_map<std::string, Texture*> textures;
    static std::unordered_map<std::string, Mix_Chunk*> sounds;
  public:
    RessourceManager() = delete;
    
    /**
     * @brief Charge tous les .png dans textures et tous les .wav dans sounds
     * mapper avec leurs chemin d'accés depuis assets (sans "assets/")
     *
     * @param racinePath Chemin vers le dossier racine du jeu
     */
    static void loadAll(std::string racinePath = "");

    /**
     * @brief Renvoie une texture 
     *
     * @param nom de la texture 
     */
    static Texture* getTexture(std::string name = "");

    /**
     * @brief Renvoie un son 
     *
     * @param nom du son
     */
    static Mix_Chunk* getSound(std::string name = "");
};
