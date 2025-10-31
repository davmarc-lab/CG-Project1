#pragma once

#include <string>

namespace ogl {

    /**
    * @brief 
    */
    class Layer {
    public:
        /**
        * @brief Instances a layer with the given name.
        *
        * @param name layer name
        */
        Layer(const std::string &name) : m_name(std::move(name)) {}

        /**
        * @brief Instances a layer with a default name.
        */
        Layer() : Layer("Simple Layer") {}

        Layer(const Layer &other) = delete;

        virtual ~Layer() = default;

        /**
        * @brief This method is called when the layer is attached.
        * It should contain all the initializations.
        */
        virtual void onAttach() {}

        /**
        * @brief This method is called when the layer is detached.
        * It should contain operations to reset the current layer.
        */
        virtual void onDetach() {}

        /**
        * @brief This method is called during game loop update.
        * These operations are called at every main loop iteration.
        */
        virtual void onUpdate() {}

        /**
        * @brief This method is called during game loop render.
        * These operations are called at every main loop iteration.
        */
        virtual void onRender() {}

        /**
        * @brief This method is called before game loop render.
        * Tipically contains specific render calls (ImGui pre rendering)
        */
        virtual void begin() {}

        /**
        * @brief This method is called after game loop render.
        * Tipically contains specific render calls (ImGui final render)
        */
        virtual void end() {}

        /**
        * @brief Retrieves the layer name.
        *
        * @return the layer name
        */
        std::string getName() const { return this->m_name; }

        /**
        * @brief Retrieves the layer state.
        *
        * @return true if the layer is attached
        */
        bool isAttached() const { return this->m_attached; }

    protected:
        /// layer name
        std::string m_name{"Layer"};
        /// layer attached state
        bool m_attached = false;
    };
} // namespace ogl
