#pragma once
#include "AssetManipulatorInterface.hpp"

class assetManipulator : public assetManipulatorInterface
{
    // Inherited via assetManipulatorInterface
    virtual const std::vector<std::wstring>& assetList() override;

    virtual std::string stringFromWide(const std::wstring& p_convert) override;

    // call this when user makes a new asset and you want the asset list to reflect that
    void reloadAssets() override;

};
