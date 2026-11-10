#pragma once
#include <string>

// https://www.uuidgenerator.net

namespace ServerSettings {
    const std::string serverName = "100ID_ESP32";
    constexpr int advertisingInterval = 100;
};

namespace GameInfoService {
    const std::string UUID = "9e7cab03-cdaf-4966-84fa-92ee0bc7d8c3";

    namespace AlgorithmStrengthCharacteristic {
        const std::string UUID = "6b85c9b7-2736-4adc-b87d-bfa97e767329";
    };

    namespace GameOverCharacteristic {
        const std::string UUID = "5fc69066-dde4-4cef-b026-82c06f86a1f6";
    };

};

namespace PlayerInfoService {
    const std::string UUID = "0478fc1f-24ec-47e9-a2ca-fc28481dd592";

    namespace GridCharacteristic {
        const std::string UUID = "a8f607fb-4f1e-491a-a28b-aac0111faf2c";
    };

    namespace LastShotCoordsCharacteristic {
        const std::string UUID = "12d64fe0-eab5-4d29-988d-504ac91448c8";
    };

    namespace LastShotResultCharacteristic {
        const std::string UUID = "35c5b4bd-2ed1-4b61-a257-1912c2358a49";
    };
};

namespace RobotInfoService {
    const std::string UUID = "9329e298-5123-45ff-a6c3-f4a5fd3889c0";

    namespace LastShotCoordsCharacteristic {
        const std::string UUID = "f153e743-20b0-48b3-b1a4-6e3fe4786710";
    };

    namespace LastShotResultCharacteristic {
        const std::string UUID = "7ec2030b-6ae2-4743-88cf-beedcad4190d";
    };

};