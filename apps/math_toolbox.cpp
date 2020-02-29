#include "math_toolbox.h"
#include "./shared/toolbox_helpers.h"
#include <assert.h>
#include <string.h>

using namespace Poincare;

/* TODO
 * Unit submenu should be created from the Poincare::Unit::Representative table.
 * This would avoid to store duplicates const char *.
 * This would requires classes as:
 * - PointerTree parent of MessageTree and BufferTree
 * - PointerTableCell instead of MessageTableCell
 *
 * We should add in the model tree the possibility to indicate a Leaf that has
 * to be selected when the menu appears. */

/* We create one model tree: each node keeps the label of the row it refers to
 * and the text which would be edited by clicking on the row. When the node is a
 * subtree, the edited text is set at I18n::Message::Default. */

const ToolboxMessageTree calculChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::DiffCommandWithArg, I18n::Message::DerivateNumber, false, I18n::Message::DiffCommand),
  ToolboxMessageTree::Leaf(I18n::Message::IntCommandWithArg, I18n::Message::Integral, false, I18n::Message::IntCommand),
  ToolboxMessageTree::Leaf(I18n::Message::SumCommandWithArg, I18n::Message::Sum, false, I18n::Message::SumCommand),
  ToolboxMessageTree::Leaf(I18n::Message::ProductCommandWithArg, I18n::Message::Product, false, I18n::Message::ProductCommand),
};

const ToolboxMessageTree complexChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::AbsCommandWithArg,I18n::Message::ComplexAbsoluteValue),
  ToolboxMessageTree::Leaf(I18n::Message::ArgCommandWithArg, I18n::Message::Agument),
  ToolboxMessageTree::Leaf(I18n::Message::ReCommandWithArg, I18n::Message::RealPart),
  ToolboxMessageTree::Leaf(I18n::Message::ImCommandWithArg, I18n::Message::ImaginaryPart),
  ToolboxMessageTree::Leaf(I18n::Message::ConjCommandWithArg, I18n::Message::Conjugate)
};

const ToolboxMessageTree combinatoricsChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::BinomialCommandWithArg, I18n::Message::Combination),
  ToolboxMessageTree::Leaf(I18n::Message::PermuteCommandWithArg, I18n::Message::Permutation)
};

const ToolboxMessageTree normalDistributionChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::NormCDFCommandWithArg, I18n::Message::NormCDF),
  ToolboxMessageTree::Leaf(I18n::Message::NormCDF2CommandWithArg, I18n::Message::NormCDF2),
  ToolboxMessageTree::Leaf(I18n::Message::InvNormCommandWithArg, I18n::Message::InvNorm),
  ToolboxMessageTree::Leaf(I18n::Message::NormPDFCommandWithArg, I18n::Message::NormPDF)
};

const ToolboxMessageTree binomialDistributionChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::BinomialPDFCommandWithArg, I18n::Message::BinomialPDF),
  ToolboxMessageTree::Leaf(I18n::Message::BinomialCDFCommandWithArg, I18n::Message::BinomialCDF),
  ToolboxMessageTree::Leaf(I18n::Message::InvBinomialCommandWithArg, I18n::Message::InvBinomial),
};

const ToolboxMessageTree probabilityChildren[] = {
  ToolboxMessageTree::Node(I18n::Message::NormalDistribution, normalDistributionChildren),
  ToolboxMessageTree::Node(I18n::Message::BinomialDistribution, binomialDistributionChildren)};

const ToolboxMessageTree arithmeticChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::GcdCommandWithArg, I18n::Message::GreatCommonDivisor),
  ToolboxMessageTree::Leaf(I18n::Message::LcmCommandWithArg, I18n::Message::LeastCommonMultiple),
  ToolboxMessageTree::Leaf(I18n::Message::FactorCommandWithArg, I18n::Message::PrimeFactorDecomposition),
  ToolboxMessageTree::Leaf(I18n::Message::RemCommandWithArg, I18n::Message::Remainder),
  ToolboxMessageTree::Leaf(I18n::Message::QuoCommandWithArg, I18n::Message::Quotient)
};

const ToolboxMessageTree matricesChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::MatrixCommandWithArg, I18n::Message::NewMatrix, false, I18n::Message::MatrixCommand),
  ToolboxMessageTree::Leaf(I18n::Message::IndentityCommandWithArg, I18n::Message::Identity),
  ToolboxMessageTree::Leaf(I18n::Message::InverseCommandWithArg, I18n::Message::Inverse),
  ToolboxMessageTree::Leaf(I18n::Message::DeterminantCommandWithArg, I18n::Message::Determinant),
  ToolboxMessageTree::Leaf(I18n::Message::TransposeCommandWithArg, I18n::Message::Transpose),
  ToolboxMessageTree::Leaf(I18n::Message::TraceCommandWithArg, I18n::Message::Trace),
  ToolboxMessageTree::Leaf(I18n::Message::DimensionCommandWithArg, I18n::Message::Dimension)
};

#if LIST_ARE_DEFINED
const ToolboxMessageTree listsChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::SortCommandWithArg, I18n::Message::Sort),
  ToolboxMessageTree::Leaf(I18n::Message::InvSortCommandWithArg, I18n::Message::InvSort),
  ToolboxMessageTree::Leaf(I18n::Message::MaxCommandWithArg, I18n::Message::Maximum),
  ToolboxMessageTree::Leaf(I18n::Message::MinCommandWithArg, I18n::Message::Minimum),
  ToolboxMessageTree::Leaf(I18n::Message::DimensionCommandWithArg, I18n::Message::Dimension)
};
#endif

const ToolboxMessageTree unitTimeSecondChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeSecondSymbol, I18n::Message::UnitTimeSecond),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeSecondMilliSymbol, I18n::Message::UnitTimeSecondMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeSecondMicroSymbol, I18n::Message::UnitTimeSecondMicro),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeSecondNanoSymbol, I18n::Message::UnitTimeSecondNano),
};

const ToolboxMessageTree unitTimeChildren[] = {
  ToolboxMessageTree::Node(I18n::Message::UnitTimeSecondMenu, unitTimeSecondChildren),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeMinuteSymbol, I18n::Message::UnitTimeMinute),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeHourSymbol, I18n::Message::UnitTimeHour),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeDaySymbol, I18n::Message::UnitTimeDay),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeWeekSymbol, I18n::Message::UnitTimeWeek),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeMonthSymbol, I18n::Message::UnitTimeMonth),
  ToolboxMessageTree::Leaf(I18n::Message::UnitTimeYearSymbol, I18n::Message::UnitTimeYear)};

const ToolboxMessageTree unitDistanceMeterChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterKiloSymbol, I18n::Message::UnitDistanceMeterKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterSymbol, I18n::Message::UnitDistanceMeter),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterMilliSymbol, I18n::Message::UnitDistanceMeterMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterMicroSymbol, I18n::Message::UnitDistanceMeterMicro),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterNanoSymbol, I18n::Message::UnitDistanceMeterNano),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceMeterPicoSymbol, I18n::Message::UnitDistanceMeterPico),
};

const ToolboxMessageTree unitDistanceChildren[] = {
  ToolboxMessageTree::Node(I18n::Message::UnitDistanceMeterMenu, unitDistanceMeterChildren),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceAstronomicalUnitSymbol, I18n::Message::UnitDistanceAstronomicalUnit),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceLightYearSymbol, I18n::Message::UnitDistanceLightYear),
  ToolboxMessageTree::Leaf(I18n::Message::UnitDistanceParsecSymbol, I18n::Message::UnitDistanceParsec)};

const ToolboxMessageTree unitMassChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassTonneSymbol, I18n::Message::UnitMassTonne),
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassGramKiloSymbol, I18n::Message::UnitMassGramKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassGramSymbol, I18n::Message::UnitMassGram),
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassGramMilliSymbol, I18n::Message::UnitMassGramMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassGramMicroSymbol, I18n::Message::UnitMassGramMicro),
  ToolboxMessageTree::Leaf(I18n::Message::UnitMassGramNanoSymbol, I18n::Message::UnitMassGramNano),
};

const ToolboxMessageTree unitCurrentAmpereChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitCurrentAmpereSymbol, I18n::Message::UnitCurrentAmpere),
  ToolboxMessageTree::Leaf(I18n::Message::UnitCurrentAmpereMilliSymbol, I18n::Message::UnitCurrentAmpereMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitCurrentAmpereMicroSymbol, I18n::Message::UnitCurrentAmpereMicro),
};

const ToolboxMessageTree unitTemperatureChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitTemperatureKelvinSymbol, I18n::Message::UnitTemperatureKelvin)};

const ToolboxMessageTree unitAmountMoleChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitAmountMoleSymbol, I18n::Message::UnitAmountMole),
  ToolboxMessageTree::Leaf(I18n::Message::UnitAmountMoleMilliSymbol, I18n::Message::UnitAmountMoleMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitAmountMoleMicroSymbol, I18n::Message::UnitAmountMoleMicro),
};

const ToolboxMessageTree unitLuminousIntensityChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitLuminousIntensityCandelaSymbol, I18n::Message::UnitLuminousIntensityCandela)};


const ToolboxMessageTree unitLuminousFluxChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitLuminousFluxLumenSymbol, I18n::Message::UnitLuminousFluxLumen),
};

const ToolboxMessageTree unitSolidAngleChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitSolidAngleSteradianSymbol, I18n::Message::UnitSolidAngleSteradian),
};

const ToolboxMessageTree unitIlluminanceChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitIlluminanceLuxSymbol, I18n::Message::UnitIlluminanceLux),
};

const ToolboxMessageTree unitFrequencyHertzChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitFrequencyHertzGigaSymbol, I18n::Message::UnitFrequencyHertzGiga),
  ToolboxMessageTree::Leaf(I18n::Message::UnitFrequencyHertzMegaSymbol, I18n::Message::UnitFrequencyHertzMega),
  ToolboxMessageTree::Leaf(I18n::Message::UnitFrequencyHertzKiloSymbol, I18n::Message::UnitFrequencyHertzKilo),
ToolboxMessageTree::Leaf(I18n::Message::UnitFrequencyHertzSymbol, I18n::Message::UnitFrequencyHertz)};

const ToolboxMessageTree unitForceNewtonChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitForceNewtonKiloSymbol, I18n::Message::UnitForceNewtonKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitForceNewtonSymbol, I18n::Message::UnitForceNewton),
  ToolboxMessageTree::Leaf(I18n::Message::UnitForceNewtonMilliSymbol, I18n::Message::UnitForceNewtonMilli),
};

const ToolboxMessageTree unitPressureChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitPressurePascalSymbol, I18n::Message::UnitPressurePascal),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPressurePascalHectoSymbol, I18n::Message::UnitPressurePascalHecto),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPressureBarSymbol, I18n::Message::UnitPressureBar),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPressureAtmSymbol, I18n::Message::UnitPressureAtm)};

const ToolboxMessageTree unitEnergyJouleChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyJouleKiloSymbol, I18n::Message::UnitEnergyJouleKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyJouleSymbol, I18n::Message::UnitEnergyJoule),
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyJouleMilliSymbol, I18n::Message::UnitEnergyJouleMilli),
};

const ToolboxMessageTree unitEnergyElectronVoltChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyElectronVoltMegaSymbol, I18n::Message::UnitEnergyElectronVoltMega),
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyElectronVoltKiloSymbol, I18n::Message::UnitEnergyElectronVoltKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyElectronVoltSymbol, I18n::Message::UnitEnergyElectronVolt),
  ToolboxMessageTree::Leaf(I18n::Message::UnitEnergyElectronVoltMilliSymbol, I18n::Message::UnitEnergyElectronVoltMilli),
};

const ToolboxMessageTree unitEnergyChildren[] = {
  ToolboxMessageTree::Node(I18n::Message::UnitEnergyJouleMenu, unitEnergyJouleChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitEnergyEletronVoltMenu, unitEnergyElectronVoltChildren)};

const ToolboxMessageTree unitPowerWattChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattGigaSymbol, I18n::Message::UnitPowerWattGiga),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattMegaSymbol, I18n::Message::UnitPowerWattMega),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattKiloSymbol, I18n::Message::UnitPowerWattKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattSymbol, I18n::Message::UnitPowerWatt),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattMilliSymbol, I18n::Message::UnitPowerWattMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPowerWattMicroSymbol, I18n::Message::UnitPowerWattMicro),
};

const ToolboxMessageTree unitElectricChargeCoulombChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitChargeCoulombSymbol, I18n::Message::UnitChargeCoulomb),
};


const ToolboxMessageTree unitPotentialVoltChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitPotentialVoltKiloSymbol, I18n::Message::UnitPotentialVoltKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPotentialVoltSymbol, I18n::Message::UnitPotentialVolt),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPotentialVoltMilliSymbol, I18n::Message::UnitPotentialVoltMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitPotentialVoltMicroSymbol, I18n::Message::UnitPotentialVoltMicro),
};

const ToolboxMessageTree unitCapacitanceFaradChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitCapacitanceFaradSymbol, I18n::Message::UnitCapacitanceFarad),
  ToolboxMessageTree::Leaf(I18n::Message::UnitCapacitanceFaradMilliSymbol, I18n::Message::UnitCapacitanceFaradMilli),
  ToolboxMessageTree::Leaf(I18n::Message::UnitCapacitanceFaradMicroSymbol, I18n::Message::UnitCapacitanceFaradMicro),
};

const ToolboxMessageTree unitResistanceOhmChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitResistanceOhmKiloSymbol, I18n::Message::UnitResistanceOhmKilo),
  ToolboxMessageTree::Leaf(I18n::Message::UnitResistanceOhmSymbol, I18n::Message::UnitResistanceOhm),
};

const ToolboxMessageTree unitConductanceSiemensChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitConductanceSiemensSymbol, I18n::Message::UnitConductanceSiemens),
  ToolboxMessageTree::Leaf(I18n::Message::UnitConductanceSiemensMilliSymbol, I18n::Message::UnitConductanceSiemensMilli),
};

const ToolboxMessageTree unitMagneticFieldChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitMagneticFieldTeslaSymbol, I18n::Message::UnitMagneticFieldTesla)};

const ToolboxMessageTree unitInductanceChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitInductanceHenrySymbol, I18n::Message::UnitInductanceHenry)};

const ToolboxMessageTree unitSurfaceChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitSurfaceHectarSymbol, I18n::Message::UnitSurfaceHectar)};

const ToolboxMessageTree unitVolumeLiterChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::UnitVolumeLiterSymbol, I18n::Message::UnitVolumeLiter),
  ToolboxMessageTree::Leaf(I18n::Message::UnitVolumeLiterDeciSymbol, I18n::Message::UnitVolumeLiterDeci),
  ToolboxMessageTree::Leaf(I18n::Message::UnitVolumeLiterCentiSymbol, I18n::Message::UnitVolumeLiterCenti),
  ToolboxMessageTree::Leaf(I18n::Message::UnitVolumeLiterMilliSymbol, I18n::Message::UnitVolumeLiterMilli),
};

const ToolboxMessageTree unitChildren[] = {
  ToolboxMessageTree::Node(I18n::Message::UnitTimeMenu, unitTimeChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitDistanceMenu, unitDistanceChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitMassMenu, unitMassChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitCurrentMenu, unitCurrentAmpereChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitTemperatureMenu, unitTemperatureChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitAmountMenu, unitAmountMoleChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitLuminousIntensityMenu, unitLuminousIntensityChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitLuminousFluxMenu, unitLuminousFluxChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitIlluminanceMenu, unitIlluminanceChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitSolidAngleMenu, unitSolidAngleChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitFrequencyMenu, unitFrequencyHertzChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitForceMenu, unitForceNewtonChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitPressureMenu, unitPressureChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitEnergyMenu, unitEnergyChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitPowerMenu, unitPowerWattChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitElectricChargeMenu, unitElectricChargeCoulombChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitPotentialMenu, unitPotentialVoltChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitCapacitanceMenu, unitCapacitanceFaradChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitResistanceMenu, unitResistanceOhmChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitConductanceMenu, unitConductanceSiemensChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitMagneticFieldMenu, unitMagneticFieldChildren),
  ToolboxMessageTree::Node(I18n::Message::InductanceMenu, unitInductanceChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitSurfaceMenu, unitSurfaceChildren),
  ToolboxMessageTree::Node(I18n::Message::UnitVolumeMenu, unitVolumeLiterChildren),
};

const ToolboxMessageTree randomAndApproximationChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::RandomCommandWithArg, I18n::Message::RandomFloat),
  ToolboxMessageTree::Leaf(I18n::Message::RandintCommandWithArg, I18n::Message::RandomInteger),
  ToolboxMessageTree::Leaf(I18n::Message::FloorCommandWithArg, I18n::Message::Floor),
  ToolboxMessageTree::Leaf(I18n::Message::FracCommandWithArg, I18n::Message::FracPart),
  ToolboxMessageTree::Leaf(I18n::Message::CeilCommandWithArg, I18n::Message::Ceiling),
  ToolboxMessageTree::Leaf(I18n::Message::RoundCommandWithArg, I18n::Message::Rounding)};

const ToolboxMessageTree trigonometryChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::CoshCommandWithArg, I18n::Message::HyperbolicCosine),
  ToolboxMessageTree::Leaf(I18n::Message::SinhCommandWithArg, I18n::Message::HyperbolicSine),
  ToolboxMessageTree::Leaf(I18n::Message::TanhCommandWithArg, I18n::Message::HyperbolicTangent),
  ToolboxMessageTree::Leaf(I18n::Message::AcoshCommandWithArg, I18n::Message::InverseHyperbolicCosine),
  ToolboxMessageTree::Leaf(I18n::Message::AsinhCommandWithArg, I18n::Message::InverseHyperbolicSine),
  ToolboxMessageTree::Leaf(I18n::Message::AtanhCommandWithArg, I18n::Message::InverseHyperbolicTangent)};

const ToolboxMessageTree predictionChildren[] = {
  ToolboxMessageTree::Leaf(I18n::Message::Prediction95CommandWithArg, I18n::Message::Prediction95),
  ToolboxMessageTree::Leaf(I18n::Message::PredictionCommandWithArg, I18n::Message::Prediction),
  ToolboxMessageTree::Leaf(I18n::Message::ConfidenceCommandWithArg, I18n::Message::Confidence)};

const ToolboxMessageTree Pka[] = {
  ToolboxMessageTree::Leaf(I18n::Message::Pka01, I18n::Message::Pka01Value, false, I18n::Message::Pka01Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka02, I18n::Message::Pka02Value, false, I18n::Message::Pka02Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka03, I18n::Message::Pka03Value, false, I18n::Message::Pka03Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka04, I18n::Message::Pka04Value, false, I18n::Message::Pka04Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka05, I18n::Message::Pka05Value, false, I18n::Message::Pka05Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka06, I18n::Message::Pka06Value, false, I18n::Message::Pka06Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka07, I18n::Message::Pka07Value, false, I18n::Message::Pka07Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka08, I18n::Message::Pka08Value, false, I18n::Message::Pka08Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka09, I18n::Message::Pka09Value, false, I18n::Message::Pka09Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka10, I18n::Message::Pka10Value, false, I18n::Message::Pka10Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka11, I18n::Message::Pka11Value, false, I18n::Message::Pka11Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka12, I18n::Message::Pka12Value, false, I18n::Message::Pka12Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka13, I18n::Message::Pka13Value, false, I18n::Message::Pka13Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka14, I18n::Message::Pka14Value, false, I18n::Message::Pka14Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka15, I18n::Message::Pka15Value, false, I18n::Message::Pka15Value),
  ToolboxMessageTree::Leaf(I18n::Message::Pka16, I18n::Message::Pka16Value, false, I18n::Message::Pka16Value),
  ToolboxMessageTree::Leaf(I18n::Message::UltimateAnswer, I18n::Message::UltimateAnswerValue, false, I18n::Message::UltimateAnswerValue),
};

const ToolboxMessageTree chemistry[] = {
  ToolboxMessageTree::Node(I18n::Message::Pka, Pka),
};

const ToolboxMessageTree FundamentalConstants[] = {
  ToolboxMessageTree::Leaf(I18n::Message::SpeedOfLightTag, I18n::Message::SpeedOfLight, false, I18n::Message::SpeedOfLight),
  ToolboxMessageTree::Leaf(I18n::Message::Vacuum_permittivityTag, I18n::Message::Vacuum_permittivity, false, I18n::Message::Vacuum_permittivity),
  ToolboxMessageTree::Leaf(I18n::Message::Vacuum_permeabilityTag, I18n::Message::Vacuum_permeability, false, I18n::Message::Vacuum_permeability),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckTag, I18n::Message::Planck, false, I18n::Message::Planck),
  ToolboxMessageTree::Leaf(I18n::Message::VacuumImpedanceTag, I18n::Message::VacuumImpedance, false, I18n::Message::VacuumImpedance),
};

const ToolboxMessageTree SpeedsOfSound[] = {
  ToolboxMessageTree::Leaf(I18n::Message::SpeedOfSound0Tag, I18n::Message::SpeedOfSound0, false, I18n::Message::SpeedOfSound0),
  ToolboxMessageTree::Leaf(I18n::Message::SpeedOfSoundWaterTag, I18n::Message::SpeedOfSoundWater, false, I18n::Message::SpeedOfSoundWater),
  ToolboxMessageTree::Leaf(I18n::Message::SpeedOfSoundGlassTag, I18n::Message::SpeedOfSoundGlass, false, I18n::Message::SpeedOfSoundGlass),
  ToolboxMessageTree::Leaf(I18n::Message::SpeedOfSoundSteelTag, I18n::Message::SpeedOfSoundSteel, false, I18n::Message::SpeedOfSoundSteel)
};

const ToolboxMessageTree EscapeVelocities[] = {
    ToolboxMessageTree::Leaf(I18n::Message::EscapeVelocityFromMoon, I18n::Message::EscapeVelocityOfMoon, false, I18n::Message::EscapeVelocityOfMoon),
  ToolboxMessageTree::Leaf(I18n::Message::EscapeVelocityFromEarth, I18n::Message::EscapeVelocityOfEarth, false, I18n::Message::EscapeVelocityOfEarth),
  ToolboxMessageTree::Leaf(I18n::Message::EscapeVelocityFromSun, I18n::Message::EscapeVelocityOfSun, false, I18n::Message::EscapeVelocityOfSun),
};

const ToolboxMessageTree Speed[] = {
  ToolboxMessageTree::Node(I18n::Message::SpeedOfSound, SpeedsOfSound),
  ToolboxMessageTree::Node(I18n::Message::EscapeVelocity, EscapeVelocities)
};

const ToolboxMessageTree Thermodynamics[] =  {
  ToolboxMessageTree::Leaf(I18n::Message::BoltzmannTag,I18n::Message::Boltzmann, false, I18n::Message::Boltzmann),
  ToolboxMessageTree::Leaf(I18n::Message::AvogadroTag, I18n::Message::Avogadro, false, I18n::Message::Avogadro),
  ToolboxMessageTree::Leaf(I18n::Message::GasTag,I18n::Message::Gas, false, I18n::Message::Gas),
  ToolboxMessageTree::Leaf(I18n::Message::StefanBoltzmannTag, I18n::Message::StefanBoltzmann, false, I18n::Message::StefanBoltzmann),
  ToolboxMessageTree::Leaf(I18n::Message::WaterTriplePointTag, I18n::Message::WaterTriplePoint, false, I18n::Message::WaterTriplePoint),
  ToolboxMessageTree::Leaf(I18n::Message::WienTag, I18n::Message::Wien, false, I18n::Message::Wien),
  ToolboxMessageTree::Leaf(I18n::Message::AtmosphericPressureTag, I18n::Message::AtmosphericPressure, false, I18n::Message::AtmosphericPressure),
};

const ToolboxMessageTree Electromagnetism[] = {
  ToolboxMessageTree::Leaf(I18n::Message::CoulombTag, I18n::Message::Coulomb, false, I18n::Message::Coulomb),
  ToolboxMessageTree::Leaf(I18n::Message::ElementalChargeTag, I18n::Message::ElementalCharge, false, I18n::Message::ElementalCharge),
  ToolboxMessageTree::Leaf(I18n::Message::FaradayConstantTag, I18n::Message::FaradayConstant, false, I18n::Message::FaradayConstant),

};

const ToolboxMessageTree ParticleMass[] = {
  ToolboxMessageTree::Leaf(I18n::Message::ElectronMassTag, I18n::Message::ElectronMass, false, I18n::Message::ElectronMass),
  ToolboxMessageTree::Leaf(I18n::Message::MuonMassTag, I18n::Message::MuonMass, false, I18n::Message::MuonMass),
  ToolboxMessageTree::Leaf(I18n::Message::NeutronMassTag, I18n::Message::NeutronMass, false, I18n::Message::NeutronMass),
  ToolboxMessageTree::Leaf(I18n::Message::ProtonMassTag, I18n::Message::ProtonMass, false, I18n::Message::ProtonMass),
  ToolboxMessageTree::Leaf(I18n::Message::TauonMassTag, I18n::Message::TauonMass, false, I18n::Message::TauonMass),
  ToolboxMessageTree::Leaf(I18n::Message::WBosonMassTag, I18n::Message::WBosonMass, false, I18n::Message::WBosonMass),
  ToolboxMessageTree::Leaf(I18n::Message::ZBosonMassTag, I18n::Message::ZBosonMass, false, I18n::Message::ZBosonMass),
  ToolboxMessageTree::Leaf(I18n::Message::AtomicMassUnitTag, I18n::Message::AtomicMassUnit, false, I18n::Message::AtomicMassUnit),
};


const ToolboxMessageTree Nuclear[] = {
  ToolboxMessageTree::Leaf(I18n::Message::FineStructureTag, I18n::Message::FineStructure, false, I18n::Message::FineStructure),
  ToolboxMessageTree::Leaf(I18n::Message::RydbergConstantTag, I18n::Message::RydbergConstant, false, I18n::Message::RydbergConstant),
  ToolboxMessageTree::Leaf(I18n::Message::HartreeConstantTag, I18n::Message::HartreeConstant, false, I18n::Message::HartreeConstant),
  ToolboxMessageTree::Leaf(I18n::Message::MagneticFluxQuantumTag, I18n::Message::MagneticFluxQuantum, false, I18n::Message::MagneticFluxQuantum),
  ToolboxMessageTree::Leaf(I18n::Message::ConductanceQuantumTag, I18n::Message::ConductanceQuantum, false, I18n::Message::ConductanceQuantum),
  ToolboxMessageTree::Leaf(I18n::Message::CirculationQuantumTag, I18n::Message::CirculationQuantum, false, I18n::Message::CirculationQuantum),
  ToolboxMessageTree::Leaf(I18n::Message::BohrRadiusTag, I18n::Message::BohrRadius, false, I18n::Message::BohrRadius),
  ToolboxMessageTree::Leaf(I18n::Message::BohrMagnetonTag, I18n::Message::BohrMagneton, false, I18n::Message::BohrMagneton),
  ToolboxMessageTree::Leaf(I18n::Message::NuclearMagnetonTag, I18n::Message::NuclearMagneton, false, I18n::Message::NuclearMagneton),
  ToolboxMessageTree::Node(I18n::Message::ParticleMass, ParticleMass),
};

const ToolboxMessageTree Gravitation[] = {
  ToolboxMessageTree::Leaf(I18n::Message::GAccelerationTag, I18n::Message::GAcceleration, false, I18n::Message::GAcceleration),
  ToolboxMessageTree::Leaf(I18n::Message::GConstantTag, I18n::Message::GConstant, false, I18n::Message::GConstant),
};


const ToolboxMessageTree Radiuses[] = {
  ToolboxMessageTree::Leaf(I18n::Message::SunMassTag, I18n::Message::SunRadius, false, I18n::Message::SunRadius),
  ToolboxMessageTree::Leaf(I18n::Message::EarthMassTag, I18n::Message::EarthRadius, false, I18n::Message::EarthRadius),
  ToolboxMessageTree::Leaf(I18n::Message::MoonMassTag, I18n::Message::MoonRadius, false, I18n::Message::MoonRadius),

};

const ToolboxMessageTree Distances[] = {
  ToolboxMessageTree::Leaf(I18n::Message::EarthMoonDistanceTag, I18n::Message::EarthMoonDistance, false, I18n::Message::EarthMoonDistance),
    ToolboxMessageTree::Leaf(I18n::Message::EarthSunDistanceTag, I18n::Message::EarthSunDistance, false, I18n::Message::EarthSunDistance),
};

const ToolboxMessageTree Length[] = {
  ToolboxMessageTree::Node(I18n::Message::Radiuses, Radiuses),
  ToolboxMessageTree::Node(I18n::Message::Distances, Distances),
};

const ToolboxMessageTree Mass[] = {
  ToolboxMessageTree::Leaf(I18n::Message::SunMassTag, I18n::Message::SunMass, false, I18n::Message::SunMass),
  ToolboxMessageTree::Leaf(I18n::Message::EarthMassTag, I18n::Message::EarthMass, false, I18n::Message::EarthMass),
  ToolboxMessageTree::Leaf(I18n::Message::MoonMassTag, I18n::Message::MoonMass, false, I18n::Message::MoonMass),
};


const ToolboxMessageTree PlanckUnits[] = {
  ToolboxMessageTree::Leaf(I18n::Message::PlanckReduceTag, I18n::Message::PlanckReduce, false, I18n::Message::PlanckReduce),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckMassTag, I18n::Message::PlanckMass, false, I18n::Message::PlanckMass),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckLengthTag, I18n::Message::PlanckLength, false, I18n::Message::PlanckLength),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckTimeTag, I18n::Message::PlanckTime, false, I18n::Message::PlanckTime),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckTemperatureTag, I18n::Message::PlanckTemperature, false, I18n::Message::PlanckTemperature),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckChargeTag, I18n::Message::PlanckCharge, false, I18n::Message::PlanckCharge),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckForceTag, I18n::Message::PlanckForce, false, I18n::Message::PlanckForce),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckEnergyTag, I18n::Message::PlanckEnergy, false, I18n::Message::PlanckEnergy),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckPowerTag, I18n::Message::PlanckPower, false, I18n::Message::PlanckPower),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckDensityTag, I18n::Message::PlanckDensity, false, I18n::Message::PlanckDensity),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckQuantityMovementTag, I18n::Message::PlanckQuantityMovement, false, I18n::Message::PlanckQuantityMovement),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckLinearMassTag, I18n::Message::PlanckLinearMass, false, I18n::Message::PlanckLinearMass),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckTensionTag, I18n::Message::PlanckTension, false, I18n::Message::PlanckTension),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckCurrentTag, I18n::Message::PlanckCurrent, false, I18n::Message::PlanckCurrent),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckPressureTag, I18n::Message::PlanckPressure, false, I18n::Message::PlanckPressure),
  ToolboxMessageTree::Leaf(I18n::Message::PlanckImpedanceTag, I18n::Message::PlanckImpedance, false, I18n::Message::PlanckImpedance),
};

const ToolboxMessageTree Physics[] = {
  ToolboxMessageTree::Node(I18n::Message::FundamentalConstants, FundamentalConstants),
  ToolboxMessageTree::Node(I18n::Message::Electromagnetism, Electromagnetism),
  ToolboxMessageTree::Node(I18n::Message::NuclearConstants, Nuclear),
  ToolboxMessageTree::Node(I18n::Message::Thermodynamics, Thermodynamics),
  ToolboxMessageTree::Node(I18n::Message::Gravitation, Gravitation),
  ToolboxMessageTree::Node(I18n::Message::Speed, Speed),
  ToolboxMessageTree::Node(I18n::Message::Mass, Mass),
  ToolboxMessageTree::Node(I18n::Message::Length, Length),
  ToolboxMessageTree::Node(I18n::Message::PlanckUnitsTag, PlanckUnits),
};



const ToolboxMessageTree menu[] = {
  ToolboxMessageTree::Leaf(I18n::Message::AbsCommandWithArg, I18n::Message::AbsoluteValue),
  ToolboxMessageTree::Leaf(I18n::Message::RootCommandWithArg, I18n::Message::NthRoot),
  ToolboxMessageTree::Leaf(I18n::Message::LogCommandWithArg, I18n::Message::BasedLogarithm),
  ToolboxMessageTree::Node(I18n::Message::Calculation, calculChildren),
  ToolboxMessageTree::Node(I18n::Message::ComplexNumber, complexChildren),
  ToolboxMessageTree::Node(I18n::Message::Combinatorics, combinatoricsChildren),
  ToolboxMessageTree::Node(I18n::Message::Probability, probabilityChildren),
  ToolboxMessageTree::Node(I18n::Message::Arithmetic, arithmeticChildren),
  ToolboxMessageTree::Node(I18n::Message::Matrices, matricesChildren),
#if LIST_ARE_DEFINED
  ToolboxMessageTree::Node(I18n::Message::Lists,listsChildren),
#endif
  ToolboxMessageTree::Node(I18n::Message::Unit, unitChildren),
  ToolboxMessageTree::Node(I18n::Message::RandomAndApproximation, randomAndApproximationChildren),
  ToolboxMessageTree::Node(I18n::Message::HyperbolicTrigonometry, trigonometryChildren),
  ToolboxMessageTree::Node(I18n::Message::Fluctuation, predictionChildren),
  ToolboxMessageTree::Node(I18n::Message::Chemistry, chemistry),
  ToolboxMessageTree::Node(I18n::Message::Physics, Physics)
  };

const ToolboxMessageTree toolboxModel = ToolboxMessageTree::Node(I18n::Message::Toolbox, menu);

MathToolbox::MathToolbox() :
  Toolbox(nullptr, rootModel()->label())
{
}

bool MathToolbox::selectLeaf(int selectedRow) {
  ToolboxMessageTree * messageTree = (ToolboxMessageTree *)m_messageTreeModel->children(selectedRow);
  m_selectableTableView.deselectTable();

  // Translate the message
  const char * text = I18n::translate(messageTree->insertedText());
  char textToInsert[k_maxMessageSize]; // Has to be in the same scope as handleEventWithText
  if (messageTree->label() == messageTree->insertedText()) {
  //  Remove the arguments if we kept one message for both inserted and displayed message
    int maxTextToInsertLength = strlen(text) + 1;
    assert(maxTextToInsertLength <= k_maxMessageSize);
    Shared::ToolboxHelpers::TextToInsertForCommandText(text, textToInsert, maxTextToInsertLength, true);
    text = textToInsert;
  }
  sender()->handleEventWithText(text);
  Container::activeApp()->dismissModalViewController();
  return true;
}

const ToolboxMessageTree * MathToolbox::rootModel() const {
  return &toolboxModel;
}

MessageTableCellWithMessage * MathToolbox::leafCellAtIndex(int index) {
  assert(index >= 0 && index < k_maxNumberOfDisplayedRows);
  return &m_leafCells[index];
}

MessageTableCellWithChevron* MathToolbox::nodeCellAtIndex(int index) {
  assert(index >= 0 && index < k_maxNumberOfDisplayedRows);
  return &m_nodeCells[index];
}

int MathToolbox::maxNumberOfDisplayedRows() {
 return k_maxNumberOfDisplayedRows;
}
