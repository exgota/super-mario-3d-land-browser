// Standalone resource-local draw-order command. No game data is embedded.
#define main assetReaderMain
#include "AssetReader.cpp"
#undef main

void writeScheduleBytes(std::ostream& output, const std::string& value) {
    output << '"' << std::hex << std::setfill('0');
    for (const unsigned char byte : value) output << std::setw(2) << unsigned(byte);
    output << '"' << std::dec;
}
void writeScheduleIndices(std::ostream& output, const std::vector<std::size_t>& values) {
    output << '[';
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index) output << ',';
        output << values[index];
    }
    output << ']';
}
int main(int argumentCount, char** arguments) {
    if (argumentCount != 4) {
        std::cerr << "Usage: model_draw_schedule <local CGFX member> <unmodified resource material: true|false|missing> <serialized mesh list: true|false|missing>\n";
        return 2;
    }
    try {
        std::ostringstream report;
        const auto data = runtime::readFile(arguments[1]);
        const auto catalog = runtime::readCgfx(data);
        runtime::CpuModelDrawScheduleInput caller;
        auto identity = [](const std::string& text) -> std::optional<bool> {
            if (text == "missing") return std::nullopt;
            runtime::require(text == "true" || text == "false", "Invalid path identity");
            return text == "true";
        };
        caller.unmodifiedResourceMaterial = identity(arguments[2]);
        caller.serializedMeshListActive = identity(arguments[3]);
        report << '[';
        for (std::size_t modelIndex = 0; modelIndex < catalog.models.size(); ++modelIndex) {
            if (modelIndex) report << ',';
            const auto& model = catalog.models[modelIndex];
            const auto actual = runtime::readCpuModelDrawSchedule(data, catalog, modelIndex, &caller);
            report << "{\"model_offset\":" << model.offset << ",\"status\":\"" << actual.status
                      << "\",\"visibility_status\":\"" << actual.visibilityStatus << "\",\"order\":";
            writeScheduleIndices(report, actual.orderedMeshIndices);
            report << ",\"visible\":";
            if (actual.serializedVisibleMeshIndices) writeScheduleIndices(report, *actual.serializedVisibleMeshIndices);
            else report << "null";
            report << ",\"entries\":[";
            for (std::size_t index = 0; index < actual.entries.size(); ++index) {
                if (index) report << ',';
                const auto& entry = actual.entries[index];
                report << "{\"mesh_index\":" << entry.meshIndex << ",\"mesh_offset\":" << entry.meshOffset
                          << ",\"material_offset\":" << entry.materialOffset << ",\"shape_offset\":" << entry.shapeOffset
                          << ",\"material_order_word\":" << entry.materialOrderWord
                          << ",\"material_order_byte\":" << unsigned(entry.materialOrderByte)
                          << ",\"mesh_order_byte\":" << unsigned(entry.meshOrderByte)
                          << ",\"visibility_byte\":" << unsigned(entry.meshVisibilityByte)
                          << ",\"signed_node_index\":" << entry.signedNodeIndex
                          << ",\"visibility_status\":\"" << entry.visibilityStatus << "\",\"serialized_visible\":";
                if (entry.serializedVisible) report << (*entry.serializedVisible ? "true" : "false");
                else report << "null";
                report << ",\"name_bytes\":"; writeScheduleBytes(report, entry.materialName);
                report << ",\"folded_name_bytes\":"; writeScheduleBytes(report, entry.foldedMaterialName);
                report << '}';
            }
            report << "]}";
        }
        report << "]\n";
        std::cout << report.str();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "model_draw_schedule: " << error.what() << '\n'; return 1;
    }
}
