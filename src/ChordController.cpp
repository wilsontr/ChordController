#include "plugin.hpp"

#define SEMITONE_STEP 0.08333333333
#define NUM_CHANNELS 6

struct ChordController : Module {
	enum ParamIds {
		SHIFT1_PARAM,
		SHIFT2_PARAM,
		SHIFT3_PARAM,
		SHIFT4_PARAM,
		SHIFT5_PARAM,
		SHIFT6_PARAM,
		QUANTIZE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		CV1_OUTPUT,
		CV2_OUTPUT,
		CV3_OUTPUT,
		CV4_OUTPUT,
		CV5_OUTPUT,
		CV6_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	ChordController() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(SHIFT1_PARAM, -3.f, 3.f, 0.f, "Shift 1");
		configParam(SHIFT2_PARAM, -3.f, 3.f, 0.f, "Shift 2");
		configParam(SHIFT3_PARAM, -3.f, 3.f, 0.f, "Shift 3");
		configParam(SHIFT4_PARAM, -3.f, 3.f, 0.f, "Shift 4");
		configParam(SHIFT5_PARAM, -3.f, 3.f, 0.f, "Shift 5");
		configParam(SHIFT6_PARAM, -3.f, 3.f, 0.f, "Shift 6");
		configParam(QUANTIZE_PARAM, 0.f, 1.f, 0.f, "Quantize");
	}

	float quantize(float pitch) {
		return (float)(floor(pitch / SEMITONE_STEP) * SEMITONE_STEP);
	}

	void process(const ProcessArgs &args) override {
		float pitchInput = inputs[PITCH_INPUT].getVoltage();
		bool quantizeOn = params[QUANTIZE_PARAM].getValue() > 0.f;

		for (int channel = 0; channel < NUM_CHANNELS; channel++) {
			float pitch = pitchInput + params[channel].getValue();
	    pitch = clamp(pitch, -4.f, 4.f);
	    if (quantizeOn) {
	    	pitch = quantize(pitch);	
	    }
	    outputs[channel].setVoltage(pitch);
		}
	}
};


struct ChordControllerWidget : ModuleWidget {
	ChordControllerWidget(ChordController *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ChordController.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(9.144, 22.223)), module, ChordController::SHIFT1_PARAM));
		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(27.434, 22.223)), module, ChordController::SHIFT2_PARAM));
		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(9.144, 41.273)), module, ChordController::SHIFT3_PARAM));
		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(27.434, 41.273)), module, ChordController::SHIFT4_PARAM));
		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(9.144, 60.323)), module, ChordController::SHIFT5_PARAM));
		addParam(createParamCentered<Davies1900hBlackKnob>(mm2px(Vec(27.434, 60.323)), module, ChordController::SHIFT6_PARAM));
		addParam(createParamCentered<NKK>(mm2px(Vec(27.434, 85.783)), module, ChordController::QUANTIZE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.144, 85.783)), module, ChordController::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.0, 101.635)), module, ChordController::CV1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.288, 101.635)), module, ChordController::CV2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.576, 101.635)), module, ChordController::CV3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.0, 117.532)), module, ChordController::CV4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.288, 117.532)), module, ChordController::CV5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.576, 117.532)), module, ChordController::CV6_OUTPUT));
	}
};


Model *modelChordController = createModel<ChordController, ChordControllerWidget>("ChordController");