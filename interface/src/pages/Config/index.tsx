import { useState } from "preact/hooks";

export default function Config() {
  const [loading, setLoading] = useState(false);

  const handleDim = async () => {
    setLoading(true);

    try {
      await fetch("https://sunrise.evailson.dev/api/dim-screen", {
        method: "POST",
      });
    } finally {
      setLoading(false);
    }
  };

  const handleBright = async () => {
    setLoading(true);

    try {
      await fetch("https://sunrise.evailson.dev/api/brighten-screen", {
        method: "POST",
      });
    } finally {
      setLoading(false);
    }
  };

  return (
    <div class="container px-10 max-w-sm py-8 flex flex-col justify-center items-center">
      <div class="w-full  flex flex-col justify-between mb-8">
        <div class="flex justify-between items-center">
          <h1 class="text-2xl leading-[48px]">Configs</h1>
          {loading && <span class="loading loading-spinner loading-md" />}
        </div>
      </div>
      <div class="w-full">
        <span class="block mb-2">Screen brightness:</span>
        <div class="flex gap-2">
          <button
            class="btn btn-sm btn-primary btn-outline min-w-24"
            disabled={loading}
            onClick={handleDim}
          >
            Dim
          </button>
          <button
            class="btn btn-sm btn-primary btn-outline min-w-24"
            disabled={loading}
            onClick={handleBright}
          >
            Bright
          </button>
        </div>
      </div>
    </div>
  );
}
