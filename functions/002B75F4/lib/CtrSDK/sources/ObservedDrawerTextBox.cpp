#include <clean/ObservedTextBoxState.h>
extern "C" {
void fn_0033DE38(const nw::lyt::TextBox*, nw::lyt::UniformVector4f*);
void fn_002CA578(nw::lyt::Drawer*);
unsigned fn_0022CBEC(const nw::lyt::ObservedRgba8*);
// Reconstructed side-effect contract. Original void/receiver-return spelling is unknown.
void fn_00244164(nw::lyt::ObservedWriter*);
void fn_002CA190(nw::lyt::ObservedWriter*);
void fn_0022CA44(nw::lyt::Drawer*, const nw::lyt::Material*, bool);
}
void nw::lyt::Drawer::SetUpTextBox(const TextBox* textBox, const Material* material,
                                  const DrawInfo& drawInfo) {
    ObservedTextCommands* commands = textBox->commands;
    if (commands->commandCount == 0)
        return;
    textMode = 4;
    textEnabled = 1;
    textReserved = 0;
    fn_0033DE38(textBox, colorBuffer);
    colorCount = 3;
    fn_002CA578(this);
    ObservedWriter* writer = &drawInfo.context->writer;
    writer->commands = commands;
    ObservedRgba8 first = textBox->firstColor;
    ObservedRgba8 second = textBox->secondColor;
    unsigned firstPacked = fn_0022CBEC(&first);
    unsigned secondPacked = fn_0022CBEC(&second);
    writer->colorMode = firstPacked == secondPacked ? 0 : 2;
    fn_00244164(writer);
    writer->textFirst = first;
    writer->textSecond = second;
    fn_00244164(writer);
    second = material->firstColor;
    first = material->secondColor;
    writer->materialFirst = second;
    writer->materialSecond = first;
    writer->alpha = textBox->alpha;
    fn_002CA190(writer);
    writer->commands = 0;
    fn_0022CA44(this, 0, true);
}
