#ifndef RAVENENGINE_GRAPHICSPIPELINE_HPP
#define RAVENENGINE_GRAPHICSPIPELINE_HPP


#include <string>
#include <memory>

#include "Pipeline.hpp"
#include "Shader.hpp"


namespace re {

    class Device;

    /**
     * @brief Graphics Pipeline wrapper
     */
    class GraphicsPipeline : public Pipeline, NonCopyable {
    public:
        GraphicsPipeline();

        GraphicsPipeline(VkDevice device, const std::string& vertName, const std::string& fragName,
                         const ConfigInfo& configInfo, const std::vector<VkDescriptorSetLayout>& layouts,
                         const std::vector<VkPushConstantRange>& constantRanges);

        ~GraphicsPipeline() override;

        /**
         * Bind Pipeline to be used in render
         * @param commandBuffer Vulkan Command buffer in recording state
         */
        void bind(VkCommandBuffer const &commandBuffer) const override;

        [[nodiscard]] VkPipeline const &getPipeline() const override;

        [[nodiscard]] VkPipelineLayout const &getLayout() const override;

        /**
         * @brief Setup the necessary configuration for create a Graphics Pipeline
         * @param configInfo ConfigInfo object to save the configuration
         * @param renderPass Valid Render Pass
         */
        static void defaultConfigInfo(ConfigInfo& configInfo, VkRenderPass renderPass);

    private:
        std::unique_ptr<Shader> vertexShader;
        std::unique_ptr<Shader> fragmentShader;
        VkDevice device{};
        VkPipeline pipeline{};
        VkPipelineLayout layout{};
    };

} // namespace re


#endif //RAVENENGINE_GRAPHICSPIPELINE_HPP
