import fetch from "node-fetch";

export const text2imgHandler = async (req, res) => {
	try {
		const response = await fetch(
			"https://stablediffusionapi.com/api/v3/text2img",
			{
				method: "POST",
				headers: {
					"Content-Type": "application/json",
				},
				body: JSON.stringify({
					key: "8dGPaiUtE9BAQWQBsdpuJzrsJtRawTvj5suboXj4CxmChqRZw0hgNp7hvrdt",
					prompt:
						"ultra realistic close up portrait ((beautiful pale cyberpunk female with heavy black eyelineshaved side haircut))",
					negative_prompt:
						"((out of frame)), ((extra fingers)), mutated hands, ((poorly drawn hands)), ((poorly drawn face)), (((mutation))), (((deformed))), (((tiling))), ((naked)), ((tile)), ((fleshpile)), ((ugly)), (((abstract))), blurry, ((bad anatomy)), ((bad proportions)), ((extra limbs)), cloned face, (((skinny))), glitchy, ((extra breasts)), ((double torso)), ((extra arms)), ((extra hands)), ((mangled fingers)), ((missing breasts)), (missing lips), ((ugly face)), ((fat)), ((extra legs)), anime",
					width: "512",
					height: "512",
					samples: "1",
					num_inference_steps: "20",
					seed: null,
					guidance_scale: 7.5,
					safety_checker: "yes",
					webhook: null,
					track_id: null,
				}),
			}
		);
		const data = await response.json();
		res.json(data);
	} catch (error) {
		console.error(error);
		res.status(500).json({ error: "Internal Server Error" });
	}
};
