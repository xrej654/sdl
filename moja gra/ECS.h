#pragma once // Zapobiega wielokrotnemu do��czaniu tego pliku nag��wkowego

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <map>
#include "SDL.h"
#include "AlgorithSAT.h"

using namespace std;

// Deklaracje klas u�ywanych w systemie ECS (Entity-Component-System)
class Component;
class Entity;
class Systems;
class Manager;

// Definiowanie typu identyfikatora komponentu
using ComponentID = size_t;

// Funkcja zwracaj�ca unikalny identyfikator dla typu komponentu
inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0; // Przechowywanie ostatniego ID
    return lastID++;
}

// Szablonowa wersja funkcji dla uzyskania identyfikatora komponentu konkretnego typu
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID(); // Tworzenie ID dla nowego typu komponentu
    return typeID;
}

// Maksymalna liczba komponent�w, jakie mo�e mie� pojedyncza jednostka
constexpr size_t maxComponents = 32;

// Struktury przechowuj�ce dane komponent�w
using ComponentBitSet = bitset<maxComponents>; // Mapa bitowa reprezentuj�ca obecno�� komponent�w
using ComponentArray = array<Component*, maxComponents>; // Tablica wska�nik�w na komponenty

// Bazowa klasa komponent�w, z metodami inicjalizacji, aktualizacji i rysowania
class Component
{
public:
    Entity* entity; // Wska�nik do jednostki, do kt�rej nale�y komponent
};

// Klasa jednostki (Entity), kt�ra posiada komponenty
class Entity
{
private:
    bool active = true; // Status aktywno�ci jednostki
    bool isPlayer = false;
    bool isEnemy = false;
    vector<unique_ptr<Component>> components; // Wektor przechowuj�cy komponenty danej jednostki

    ComponentArray componentArray; // Tablica wska�nik�w na komponenty
    ComponentBitSet componentBitSet; // Mapa bitowa okre�laj�ca obecno�� komponent�w
public:
    void setIsPlayer()
    {
        isPlayer = true;
    }

    bool getIsPlayer() { return isPlayer; }

    void setIsEnemy()
    {
        isEnemy = true;
    }

    bool getIsEnemy() { return isEnemy; }

    // Sprawdzanie aktywno�ci jednostki
    bool isActive() { return active; }

    // Dezaktywacja jednostki
    void destroy() { active = false; }

    // Sprawdzenie czy jednostka posiada dany komponent
    template <typename T> bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    // Dodawanie komponentu do jednostki
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&...mArgs)
    {
        if (hasComponent<T>())
        {
            throw runtime_error("Component already exists"); // B��d, je�li komponent ju� istnieje
        }

        T* c(new T(forward<TArgs>(mArgs)...)); // Tworzenie nowego komponentu
        c->entity = this; // Ustawienie jednostki dla komponentu
        unique_ptr<Component> uPtr{ c };
        components.emplace_back(move(uPtr)); // Przeniesienie do listy komponent�w

        componentArray[getComponentTypeID<T>()] = c; // Przechowywanie wska�nika do komponentu
        componentBitSet[getComponentTypeID<T>()] = true; // Oznaczenie komponentu jako obecnego

        return *c;
    }

    // Pobieranie komponentu danego typu
    template <typename T> T& getComponent() const
    {
        if (!hasComponent<T>())
        {
            throw runtime_error("Component does not exist"); // B��d, je�li komponent nie istnieje
        }

        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr); // Konwersja wska�nika na typ komponentu
    }
};

// Klasa system�w ECS
class Systems {
public:
    Systems() {}

    // Aktualizacja system�w
    void update(Manager& manager, SDL_Renderer* ren, float deltaTime, const Uint8* keys, Uint32 mouseButtons, float mouseX, float mouseY)
    {
        movementSystem(manager, deltaTime, keys, ren);
        atackSystem(manager, mouseButtons, mouseX, mouseY);
        collisionSystem(manager);
    }

    // Deklaracje system�w obs�uguj�cych r�ne mechaniki gry
    static void movementSystem(Manager& manager, float deltaTime, const Uint8* keys, SDL_Renderer* ren);
    static void renderingSystem(Manager& manager, SDL_Renderer* ren);
    static void atackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY);
    static void collisionSystem(Manager& manager);
};

// Klasa zarz�dzaj�ca jednostkami i systemami
class Manager
{
private:
    Systems system;
    vector<unique_ptr<Entity>> entities; // Lista wszystkich jednostek
    vector<unique_ptr<Entity>> obstacles; // Lista wszystkich jednostek
public:
    // Aktualizacja jednostek i system�w
    void update(Manager& manager, SDL_Renderer* ren, float deltaTime, const Uint8* keys, Uint32 mouseButtons, float mouseX, float mouseY)
    {
        system.update(manager, ren, deltaTime, keys, mouseButtons, mouseX, mouseY);
        refresh();
    }

    // Rysowanie jednostek i system�w
    void draw(Manager& manager, SDL_Renderer* ren)
    {
        system.renderingSystem(manager, ren);
    }

    // Usuwanie nieaktywnych jednostek
    void refresh()
    {
        entities.erase(remove_if(begin(entities), end(entities),
            [](const unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isActive();
            }),
            end(entities));

        obstacles.erase(remove_if(begin(obstacles), end(obstacles),
            [](const unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isActive();
            }),
            end(obstacles));
    }

    // Tworzenie nowej jednostki
    Entity& addEntity()
    {
        Entity* e = new Entity();
        unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(move(uPtr));
        return *e;
    }

    // Tworzenie nowej przeszkody
    Entity& addObstacle()
    {
        Entity* e = new Entity();
        unique_ptr<Entity> uPtr{ e };
        obstacles.emplace_back(move(uPtr));
        return *e;
    }

    // Pobieranie listy jednostek
    vector<unique_ptr<Entity>>& getVectorOfEntities() { return entities; }
    vector<unique_ptr<Entity>>& getVectorOfObstacles() { return obstacles; }

};
