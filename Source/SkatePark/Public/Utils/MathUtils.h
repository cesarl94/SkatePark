// This class wasn't written during this technical assesstment, instead, was taken of previous projects

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MathUtils.generated.h"

UCLASS()
class SKATEPARK_API UMathUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * AKA InverseLerp
	 * if you give me an factor0Param I return an 0.
	 * if you give me an factor1Param I return an 1.
	 * then you can give me any input and I will return a value between 0 and 1 according to the values passed before.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double LinearStep(double factor0Param, double factor1Param, double input, bool clamp) {
		const double denom = factor1Param - factor0Param;
		if (denom == 0) {
			return input < factor0Param ? 0 : 1;
		}
		const double t = (input - factor0Param) / denom;
		return clamp ? FMath::Clamp(t, 0, 1) : t;
	}

	/**
	 * if you give me an inputDataA I return an outputDataA.
	 * if you give me an inputDataB I return an outputDataB.
	 * then you can give me any input and I will return a value according to the values passed before.
	 * If clamp is true, the returns values will be held between outputDataA and outputDataB
	 *
	 * If inputDataA is 0 and inputDataB is 1, you should use lerp instead this.
	 * If outputDataA is 0 and outputDataB is 1, you should use linearstep instead this.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double RuleOfFive(double inputDataA, double outputDataA, double inputDataB, double outputDataB, double input, bool clamp) {
		const double t = LinearStep(inputDataA, inputDataB, input, clamp);
		return FMath::Lerp(outputDataA, outputDataB, t);
	}

	/**
	 * Calculates and returns the alpha value for Framerate-independent Lerp Smoothing. By Freya.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 *
	 * @param dt - DeltaTime, in seconds.
	 * @param HalfLife - Time until halfway, in seconds. You can calculate this value with the function CalculateSmoothLerpHalfLife
	 *
	 * @return Smoothed alpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double GetSmoothLerpAlpha(double dt, double HalfLife) { return 1.f - FGenericPlatformMath::Exp2(-dt / HalfLife); }

	/**
	 * Calculates the HalfLife that you need for using GetSmoothLerpAlpha for a Framerate-independent Lerp Smoothing. By Freya.
	 * We recommend you calcule this value and save, to avoid calculate the same value each frame. This uses a logarithmic function.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 * https://twitter.com/FreyaHolmer/status/1757857332187324505/photo/1
	 *
	 * @param IdealFrameRate - Framerate of the game.
	 * @param IdealLerpFactor - If your Framerate-DEPENDENT LerpSmooth function was lerp(a, b, m), is "m" the value that goes here
	 *
	 * @return HalfLife that you should use with the function GetSmoothLerpAlpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double CalculateSmoothLerpHalfLife(int32 IdealFrameRate, double IdealLerpFactor) { return (-1.0 / static_cast<double>(IdealFrameRate)) / FGenericPlatformMath::Log2(1.0 - IdealLerpFactor); }
};
